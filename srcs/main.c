/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyudris <fyudris@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 09:30:17 by fyudris           #+#    #+#             */
/*   Updated: 2025/11/28 10:15:23 by fyudris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

// Temporary cleanup function for testing
void	cleanup_test(t_table *table)
{
int	i;

	i = 0;
	while (i < table->philo_nbr)
	{
		pthread_mutex_destroy(&table->forks[i]);
		// NEW: Free individual philos
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
	t_table	table;
	int		i;

	if (argc < 5 || argc > 6)
		return (error_exit("Wrong argument count."));
	if (init_data(&table, argv) != 0)
		return (1);
	
	printf("--- STEP 3 TEST: RUNNING SIMULATION (2 seconds) ---\n");
	
	// Start threads
	if (start_simulation(&table) != 0)
		return (1);

	// Let them run for 2 seconds
	precise_usleep(2000, &table);

	// Force stop
	pthread_mutex_lock(&table.sim_lock);
	table.sim_running = false;
	pthread_mutex_unlock(&table.sim_lock);

	// Join threads (wait for them to exit)
	i = 0;
	while (i < table.philo_nbr)
	{
		pthread_join(table.philos[i]->thread_id, NULL);
		i++;
	}
	printf("--- SIMULATION ENDED ---\n");

	cleanup_test(&table);
	return (0);
}