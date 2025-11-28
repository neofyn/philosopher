/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyudris <fyudris@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 10:49:15 by fyudris           #+#    #+#             */
/*   Updated: 2025/11/28 10:49:50 by fyudris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/**
 * @brief Checks if a philosopher has died.
 * Locks the meal mutex to read data safely.
 * @param philo The philosopher to check.
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
 * Constantly checks for death or if everyone ate enough.
 * @param ptr Pointer to the table struct.
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
			
		// Sleep a tiny bit to reduce CPU load (optional but recommended)
		usleep(1000);
	}
	return (NULL);
}