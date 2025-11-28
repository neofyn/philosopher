/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyudris <fyudris@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 09:30:17 by fyudris           #+#    #+#             */
/*   Updated: 2025/11/28 10:50:30 by fyudris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

// Define a proper cleanup function in cleanup.c ideally, 
// but we keep it here or utils for now.
void	cleanup_resources(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->philo_nbr)
	{
		pthread_mutex_destroy(&table->forks[i]);
		pthread_mutex_destroy(&table->philos[i]->meal_lock);
		free(table->philos[i]);
		i++;
	}
	pthread_mutex_destroy(&table->write_lock);
	pthread_mutex_destroy(&table->sim_lock);
	free(table->forks);
	free(table->philos);
}

int	main(int argc, char **argv)
{
	t_table		table;
	pthread_t	monitor_thread;
	int			i;

	if (argc < 5 || argc > 6)
		return (error_exit("Wrong argument count."));
	if (init_data(&table, argv) != 0)
		return (1);
	
	// 1. Start Philosopher Threads
	if (start_simulation(&table) != 0)
		return (1);

	// 2. Start Monitor Thread
	// We run this in a separate thread so main can join everything later.
	// Alternatively, you can just run the monitor loop directly in main here.
	if (pthread_create(&monitor_thread, NULL, \
			monitor_routine, &table) != 0)
		return (error_exit("Monitor thread creation failed."));

	// 3. Wait for simulation to end (Join Monitor)
	// The monitor only returns when someone dies or everyone eats.
	pthread_join(monitor_thread, NULL);

	// 4. Join Philosopher Threads
	i = 0;
	while (i < table.philo_nbr)
	{
		pthread_join(table.philos[i]->thread_id, NULL);
		i++;
	}

	cleanup_resources(&table);
	return (0);
}