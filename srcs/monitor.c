/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyudris <fyudris@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 10:49:15 by fyudris           #+#    #+#             */
/*   Updated: 2025/11/28 20:41:33 by fyudris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/**
 * @brief Checks if a philosopher has died.
 *
 * A philosopher is considered dead if the time elapsed since their
 * `last_meal_time` exceeds `time_to_die`. This check is thread-safe as it
 * locks the philosopher's `meal_lock` before reading `last_meal_time`.
 *
 * @param philo The philosopher to check.
 * @return `true` if dead, `false` if alive.
 */
static int	philo_died(t_philo *philo)
{
	long	elapsed;
	long	time_to_die;

	if (philo->table->time_die == -1)
		return (0);
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
 * @return `true` if simulation should stop (everyone full), `false`
 *         otherwise.
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
 * @brief Iterates through philosophers to check for any deaths.
 *
 * If a dead philosopher is found, this function sets the simulation end flag,
 * prints the death message, and signals that the simulation should stop.
 * The `write_lock` ensures the death message is printed without interruption.
 *
 * @param table The main data structure.
 * @return `true` if a death occurred, `false` otherwise.
 */
static bool	check_for_deaths(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->philo_nbr)
	{
		if (philo_died(table->philos[i]))
		{
			pthread_mutex_lock(&table->sim_lock);
			table->sim_running = false;
			pthread_mutex_unlock(&table->sim_lock);
			pthread_mutex_lock(&table->write_lock);
			printf("%ld %d died\n", get_time() - table->start_time,
				table->philos[i]->id);
			pthread_mutex_unlock(&table->write_lock);
			return (true);
		}
		i++;
	}
	return (false);
}

/**
 * @brief The monitor thread routine.
 *
 * This function runs in a separate thread and acts as the simulation's
 * referee. It continuously loops to check for the two end conditions:
 * 1.  **Death**: If any philosopher has died.
 * 2.  **All Ate**: If all philosophers have eaten the required number of meals.
 * The loop terminates as soon as one of these conditions is met.
 *
 * @param ptr Pointer to the table struct.
 * @return NULL.
 */
void	*monitor_routine(void *ptr)
{
	t_table	*table;

	table = (t_table *)ptr;
	while (true)
	{
		if (check_for_deaths(table) || check_if_all_ate(table))
			return (NULL);
		usleep(100);
	}
	return (NULL);
}
