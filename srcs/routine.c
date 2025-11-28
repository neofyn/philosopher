/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyudris <fyudris@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 10:09:02 by fyudris           #+#    #+#             */
/*   Updated: 2025/11/28 11:41:13 by fyudris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/**
 * @brief Eating routine.
 * 1. Locks forks in the correct order.
 * 2. Updates last_meal_time (protected by meal_lock).
 * 3. Prints "is eating".
 * 4. Sleeps for time_eat.
 * 5. Unlocks forks.
 * @param philo The philosopher.
 */
static void eat(t_philo *philo)
{
	pthread_mutex_lock(philo->fork_first);
	write_status(philo, "has taken a fork");

	// Edge case: One philosopher logic
	if (philo->table->philo_nbr == 1)
	{
		precise_usleep(philo->table->time_die, philo->table);
		pthread_mutex_unlock(philo->fork_first);
		return;
	}

	pthread_mutex_lock(philo->fork_second);
	write_status(philo, "has taken a fork");

	write_status(philo, "is eating");
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_meal_time = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_lock);

	precise_usleep(philo->table->time_eat, philo->table);

	pthread_mutex_unlock(philo->fork_second);
	pthread_mutex_unlock(philo->fork_first);
}

/**
 * @brief Main philosopher thread loop.
 * Cycles through Eat -> Sleep -> Think.
 * @param ptr Cast to t_philo pointer.
 * @return NULL
 */
void *philo_routine(void *ptr)
{
	t_philo *philo;

	philo = (t_philo *)ptr;
	// 1. Better Start Stagger: Wait 10% of eat time to reduce start collisions
	if (philo->id % 2 == 0)
		precise_usleep(philo->table->time_eat / 10, philo->table);

	while (1)
	{
		// Check simulation end
		pthread_mutex_lock(&philo->table->sim_lock);
		if (!philo->table->sim_running)
		{
			pthread_mutex_unlock(&philo->table->sim_lock);
			break;
		}
		pthread_mutex_unlock(&philo->table->sim_lock);

		eat(philo);

		// Check simulation end again
		pthread_mutex_lock(&philo->table->sim_lock);
		if (!philo->table->sim_running)
		{
			pthread_mutex_unlock(&philo->table->sim_lock);
			break;
		}
		pthread_mutex_unlock(&philo->table->sim_lock);

		write_status(philo, "is sleeping");
		precise_usleep(philo->table->time_sleep, philo->table);

		write_status(philo, "is thinking");

		// --- DYNAMIC THINK TIME FIX ---
		// If odd number of philos, we must wait to prevent starvation.
		if (philo->table->philo_nbr % 2 != 0)
		{
			long think_time;

			// Calculate how much "slack" exists in the cycle
			// Logic: If eat >= sleep, we must wait to let neighbor eat
			long time_eat = philo->table->time_eat;
			long time_sleep = philo->table->time_sleep;

			if (time_eat >= time_sleep)
				think_time = (time_eat * 2) - time_sleep;
			else
				think_time = 0;

			// Wait 40% of the theoretical slack to be safe
			// This prevents "greedy" behavior
			if (think_time > 0)
				precise_usleep(think_time * 0.4, philo->table);
		}
		// ------------------------------
	}
	return (NULL);
}
