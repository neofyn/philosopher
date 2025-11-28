/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyudris <fyudris@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 09:30:17 by fyudris           #+#    #+#             */
/*   Updated: 2025/11/28 11:08:36 by fyudris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	main(int argc, char **argv)
{
	t_table		table;
	pthread_t	monitor_thread;
	int			i;

	if (argc < 5 || argc > 6)
		return (error_exit("Wrong argument count."));
	
	// 1. Initialize Data
	if (init_data(&table, argv) != 0)
		return (1);
	
	// 2. Start Philosopher Threads
	if (start_simulation(&table) != 0)
		return (1);

	// 3. Start Monitor Thread
	if (pthread_create(&monitor_thread, NULL, monitor_routine, &table) != 0)
		return (error_exit("Monitor thread creation failed."));

	// 4. Wait for Monitor to finish (Death or All Ate)
	pthread_join(monitor_thread, NULL);

	// 5. Wait for Philosophers to finish
	i = 0;
	while (i < table.philo_nbr)
	{
		pthread_join(table.philos[i]->thread_id, NULL);
		i++;
	}

	// 6. Clean up
	cleanup(&table);
	return (0);
}