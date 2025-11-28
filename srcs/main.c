/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyudris <fyudris@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 09:30:17 by fyudris           #+#    #+#             */
/*   Updated: 2025/11/28 09:07:45 by fyudris          ###   ########.fr       */
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
		i++;
	}
	pthread_mutex_destroy(&table->write_lock);
	pthread_mutex_destroy(&table->sim_lock);
	if (table->forks)
		free(table->forks);
	if (table->philos)
		free(table->philos);
}

int	main(int argc, char **argv)
{
	t_table	table;

	if (argc < 5 || argc > 6)
		return (error_exit("Wrong argument count."));
	if (init_data(&table, argv) != 0)
		return (1);

	// --- TEST PRINT ---
	printf("SUCCESS! Table initialized:\n");
	printf("Philos: %ld\n", table.philo_nbr);
	printf("Time to Die: %ld\n", table.time_die);
	printf("Time to Eat: %ld\n", table.time_eat);
	printf("Time to Sleep: %ld\n", table.time_sleep);
	printf("Must Eat: %ld\n", table.must_eat_count);
	// ------------------

	cleanup_test(&table);
	return (0);
}