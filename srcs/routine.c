/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyudris <fyudris@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 10:09:02 by fyudris           #+#    #+#             */
/*   Updated: 2025/11/28 20:38:37 by fyudris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/**
 * @brief Eating routine.
 *
 * A philosopher attempts to eat by performing the following steps:
 * 1. Locks their assigned forks in the predetermined order to prevent deadlock.
 * 2. If there is only one philosopher, they will take their fork, wait until
 *    they die (as they cannot eat), and then release the fork.
 * 3. Once both forks are acquired, the philosopher's status is updated to
 *    "is eating".
 * 4. The `last_meal_time` and `meals_eaten` count are updated. This action is
 *    protected by the `meal_lock` mutex to prevent data races with the monitor.
 * 5. The philosopher "eats" for `time_eat` milliseconds.
 * 6. The forks are unlocked in the reverse order they were locked.
 *
 * @param philo The philosopher.
 */
static void	eat(t_philo *philo)
{
	pthread_mutex_lock(philo->fork_first);
	write_status(philo, "has taken a fork");
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
 * @brief The thinking routine, with a dynamic delay for odd-numbered groups.
 *
 * For an odd number of philosophers, a calculated delay is added during the
 * thinking phase. This prevents a "greedy" philosopher from repeatedly eating
 * while their neighbors starve by giving them a chance to acquire forks.
 *
 * @param philo The philosopher.
 */
static void	think(t_philo *philo)
{
	long	time_eat;
	long	time_sleep;
	long	think_time;

	write_status(philo, "is thinking");
	if (philo->table->philo_nbr % 2 != 0)
	{
		time_eat = philo->table->time_eat;
		time_sleep = philo->table->time_sleep;
		if (time_eat >= time_sleep)
			think_time = (time_eat * 2) - time_sleep;
		else
			think_time = 0;
		if (think_time > 0)
			precise_usleep(think_time * 0.4, philo->table);
	}
}

/**
 * @brief Checks if the simulation is still running.
 *
 * Safely checks the `sim_running` flag. This function is used to determine if
 * the main philosopher loop should terminate.
 *
 * @param philo The philosopher.
 * @return `true` if the simulation is running, `false` otherwise.
 */
static bool	sim_is_running(t_philo *philo)
{
	bool	running;

	pthread_mutex_lock(&philo->table->sim_lock);
	running = philo->table->sim_running;
	pthread_mutex_unlock(&philo->table->sim_lock);
	return (running);
}

/**
 * @brief The main routine for each philosopher thread.
 *
 * The routine follows a continuous cycle of eating, sleeping, and thinking.
 * 1.  **Staggered Start**: Even-ID philosophers wait briefly to prevent all
 *     threads from competing for forks at the exact same moment.
 * 2.  **Main Loop**: The philosopher continuously checks if the simulation is
 *     still running. If so, it proceeds with the eat-sleep-think cycle.
 *
 * @param ptr A void pointer to the philosopher's `t_philo` struct.
 * @return NULL
 */
void	*philo_routine(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	if (philo->id % 2 == 0)
		precise_usleep(philo->table->time_eat / 10, philo->table);
	while (sim_is_running(philo))
	{
		eat(philo);
		write_status(philo, "is sleeping");
		precise_usleep(philo->table->time_sleep, philo->table);
		think(philo);
	}
	return (NULL);
}
