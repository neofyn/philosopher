/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyudris <fyudris@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 09:39:26 by fyudris           #+#    #+#             */
/*   Updated: 2025/11/28 20:39:25 by fyudris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/**
 * @brief Gets the current time in milliseconds.
 *
 * @return The timestamp in milliseconds.
 */
long	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (0);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/**
 * @brief Sleeps for a specific duration in milliseconds.
 * Checks continuously if the simulation has stopped to allow early exit.
 *
 * @param time_in_ms Duration to sleep.
 * @param table The main data structure (to check stop flag).
 */
void	precise_usleep(long time_in_ms, t_table *table)
{
	long	start_time;

	start_time = get_time();
	while ((get_time() - start_time) < time_in_ms)
	{
		(void)table;
		usleep(500);
	}
}
