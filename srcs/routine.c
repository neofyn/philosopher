/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyudris <fyudris@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 10:09:02 by fyudris           #+#    #+#             */
/*   Updated: 2025/11/28 11:16:32 by fyudris          ###   ########.fr       */
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
static void	eat(t_philo *philo)
{
	pthread_mutex_lock(philo->fork_first);
	write_status(philo, "has taken a fork");
	
	// Edge case: One philosopher logic
	if (philo->table->philo_nbr == 1)
	{
		precise_usleep(philo->table->time_die, philo->table);
		pthread_mutex_unlock(philo->fork_first);
		return ;
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
void	*philo_routine(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	// Sync start: Even philosophers wait a bit to let Odd ones grab forks first.
	// This reduces contention at the very start.
	if (philo->id % 2 == 0)
		precise_usleep(1, philo->table);
	
	while (1)
	{
		// Check global simulation flag
		pthread_mutex_lock(&philo->table->sim_lock);
		if (!philo->table->sim_running)
		{
			pthread_mutex_unlock(&philo->table->sim_lock);
			break ;
		}
		pthread_mutex_unlock(&philo->table->sim_lock);

		eat(philo);
		
		// If simulation stopped during eating (e.g. death), exit loop
		pthread_mutex_lock(&philo->table->sim_lock);
		if (!philo->table->sim_running)
		{
			pthread_mutex_unlock(&philo->table->sim_lock);
			break ;
		}
		pthread_mutex_unlock(&philo->table->sim_lock);

		write_status(philo, "is sleeping");
		precise_usleep(philo->table->time_sleep, philo->table);
		
		write_status(philo, "is thinking");
		// --- FIX ---
		// If the number of philosophers is ODD, the cycle is not perfect.
		// We force a small wait to prevent a philosopher from immediately 
		// stealing the forks back from a starving neighbor.
		if (philo->table->philo_nbr % 2 != 0)
		{
			precise_usleep(1, philo->table); 
		}
		// -----------------------
	}
	return (NULL);
}
