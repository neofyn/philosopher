/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyudris <fyudris@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 09:30:17 by fyudris           #+#    #+#             */
/*   Updated: 2025/11/28 09:51:39 by fyudris          ###   ########.fr       */
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

	printf("--- STEP 2 TEST: FORK ASSIGNMENT ---\n");
	i = 0;
	while (i < table.philo_nbr)
	{
		printf("Philo %d (Addr: %p)\n", table.philos[i]->id, (void *)table.philos[i]);
		printf("  First Fork:  %p\n", (void *)table.philos[i]->fork_first);
		printf("  Second Fork: %p\n", (void *)table.philos[i]->fork_second);
		i++;
	}
	printf("------------------------------------\n");
	printf("Time check: %ld ms\n", get_time());
	precise_usleep(100, &table);
	printf("Time check (+100ms): %ld ms\n", get_time());

	cleanup_test(&table);
	return (0);
}