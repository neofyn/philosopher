/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyudris <fyudris@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 10:49:15 by fyudris           #+#    #+#             */
/*   Updated: 2025/11/28 20:21:02 by fyudris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/**
 * @brief Checks if a philosopher has died.
 *
 * A philosopher is considered dead if the time elapsed since their
 * `last_meal_time` exceeds `time_to_die`. This check is thread-safe as it
 * locks the philosopher's `meal_lock` before reading `last_meal_time`.
 * @param philo The philosopher to check.
 *
 * @return 1 if dead, 0 if alive.
 */
static int	philo_died(t_philo *philo)
{
	long	elapsed;
	long	time_to_die;

	if (philo->table->time_die == -1)
		return (0);

	// LOCK to read last_meal_time
	pthread_mutex_lock(&philo->meal_lock);
	elapsed = get_time() - philo->last_meal_time;
	time_to_die = philo->table->time_die;
	pthread_mutex_unlock(&philo->meal_lock);

	if (elapsed >= time_to_die)
		return (1);
	return (0);
}

/**
 * @brief Checks if all philosophers have eaten the required amount of meals.
 *
 * This function is only active if `must_eat_count` was specified. It iterates
 * through all philosophers, safely checking their `meals_eaten` count. If all
 * have reached the target, it sets the `sim_running` flag to `false` to end
 * the simulation.
 *
 * @param table The main data structure.
 * @return 1 if simulation should stop (everyone full), 0 otherwise.
 */
static int	check_if_all_ate(t_table *table)
{
	int	i;
	int	finished_eating;

	if (table->must_eat_count == -1)
		return (0);

	finished_eating = 0;
	i = 0;
	while (i < table->philo_nbr)
	{
		pthread_mutex_lock(&table->philos[i]->meal_lock);
		if (table->philos[i]->meals_eaten >= table->must_eat_count)
			finished_eating++;
		pthread_mutex_unlock(&table->philos[i]->meal_lock);
		i++;
	}

	if (finished_eating == table->philo_nbr)
	{
		pthread_mutex_lock(&table->sim_lock);
		table->sim_running = false;
		pthread_mutex_unlock(&table->sim_lock);
		return (1);
	}
	return (0);
}

/**
 * @brief The monitor thread routine.
 *
 * This function runs in a separate thread and acts as the simulation's
 * referee. It continuously loops through all philosophers to check for the two
 * end conditions:
 * 1.  **Death**: If any philosopher has died, it prints the death message,
 *     sets the `sim_running` flag to `false`, and terminates.
 * 2.  **All Ate**: If all philosophers have eaten the required number of meals,
 *     it terminates.
 *
 * @param ptr Pointer to the table struct.
 *
 * @return NULL.
 */
void	*monitor_routine(void *ptr)
{
	t_table	*table;
	int		i;

	table = (t_table *)ptr;
	while (1)
	{
		i = 0;
		while (i < table->philo_nbr)
		{
			// 1. Check Death
			if (philo_died(table->philos[i]))
			{
				// Stop the flag
				pthread_mutex_lock(&table->sim_lock);
				table->sim_running = false;
				pthread_mutex_unlock(&table->sim_lock);

				// Print death message directly (bypassing write_status check)
				pthread_mutex_lock(&table->write_lock);
				printf("%ld %d died\n", get_time() - table->start_time, \
					table->philos[i]->id);
				pthread_mutex_unlock(&table->write_lock);
				return (NULL);
			}
			i++;
		}
		// 2. Check if everyone is full
		if (check_if_all_ate(table))
			return (NULL);

		// Sleep a tiny bit to reduce CPU load
		usleep(100);
	}
	return (NULL);
}
