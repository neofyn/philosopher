/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyudris <fyudris@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 09:39:26 by fyudris           #+#    #+#             */
/*   Updated: 2025/11/28 20:20:01 by fyudris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/**
 * @brief Gets the current time in milliseconds.
 *
 * This function uses `gettimeofday` to retrieve the current time with
 * microsecond precision and then converts it to milliseconds.
 *
 * @return The current timestamp in milliseconds since the Unix epoch.
 *         Returns 0 on error, though `gettimeofday` rarely fails.
 */
long	get_time(void)
{
	struct timeval	tv;

	if(gettimeofday(&tv, NULL))
		return (0);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/**
 * @brief A more precise sleep function that actively waits for a duration.
 *
 * Standard `usleep` can be imprecise and may sleep longer than requested.
 * This function provides a more accurate delay by sleeping in very short
 * intervals (500 microseconds) within a loop, continuously checking the
 * elapsed time. This ensures that actions like eating and sleeping adhere
 * closely to the required timings.
 *
 * Note: The `table` parameter is currently unused, but it could be used to
 * check `table->sim_running` inside the loop. This would allow a sleeping
 * philosopher to wake up and terminate immediately if the simulation ends,
 * rather than completing the full sleep duration.
 *
 * @param time_in_ms Duration to sleep.
 * @param table The main data structure.
 */
void	precise_usleep(long time_in_ms, t_table *table)
{
	long	start_time;

	start_time = get_time();
	while (get_time() - start_time < time_in_ms)
	{
		(void)table; // Currently unused, see function documentation.
		usleep(500);
	}
}
