/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyudris <fyudris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 09:21:00 by fyudris           #+#    #+#             */
/*   Updated: 2025/08/12 00:24:07 by fyudris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/philo.h"

/**
 * @brief Gets the current time in milliseconds.
 * @return The current time in milliseconds since the Unix epoch.
 */
long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/**
 * @brief A more precise version of usleep that checks for simulation stop.
 * @param ms_to_sleep The duration to sleep in milliseconds.
 * @param program Pointer to the main program structure.
 *
 * This function sleeps for small intervals, checking if the simulation
 * should stop after each interval. This ensures a philosopher can react
 * quickly if another one dies during its sleep or meal.
 */
void	smart_sleep(long ms_to_sleep, t_program *program)
{
	long	start;

	start = get_time();
	while (1)
	{
		pthread_mutex_lock(&program->death_lock);
		if (program->stop_simulation)
		{
			pthread_mutex_unlock(&program->death_lock);
			break ;
		}
		pthread_mutex_unlock(&program->death_lock);
		if (get_time() - start >= ms_to_sleep)
			break ;
		usleep(100);
	}
}

/**
 * @brief Prints a status message for a philosopher.
 * @param philo The philosopher whose status is being printed.
 * @param status_msg The message to print (e.g., "is eating").
 *
 * This function locks the print mutex to ensure messages are not interleaved.
 * It also checks the simulation stop flag before printing.
 */
void	print_status(t_philo *philo, const char *status_msg)
{
	long	timestamp;

	pthread_mutex_lock(&philo->program->print_lock);
	pthread_mutex_lock(&philo->program->death_lock);
	if (!philo->program->stop_simulation)
	{
		timestamp = get_time() - philo->program->start_time;
		printf("%ld %d %s\n", timestamp, philo->id, status_msg);
	}
	pthread_mutex_unlock(&philo->program->death_lock);
	pthread_mutex_unlock(&philo->program->print_lock);
}
