/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyudris <fyudris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 09:30:17 by fyudris           #+#    #+#             */
/*   Updated: 2025/08/12 00:30:33 by fyudris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Cleans up all allocated memory and destroys mutexes.
 * @param program The main program structure.
 */
void	cleanup(t_program *program)
{
	int	i;

	i = 0;
	while (i < program->num_of_philos)
	{
		pthread_mutex_destroy(&program->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&program->print_lock);
	pthread_mutex_destroy(&program->death_lock);
	if (program->philos)
		free(program->philos);
	if (program->forks)
		free(program->forks);
}

int	main(int argc, char **argv)
{
	t_program	program;

	if (argc < 5 || argc > 6)
	{
		printf("Usage: ./philo num_philos time_die time_eat time_sleep [num_meals]\n");
		return (1);
	}
	memset(&program, 0, sizeof(t_program));
	if (init_program(&program, argc, argv) != 0)
	{
		printf("Error: Invalid input or initialization failed.\n");
		cleanup(&program); // Clean up partially allocated resources
		return (1);
	}
	
	// ---- THREAD CREATION AND MONITORING WILL GO HERE ----
	printf("Initialization successful. Simulation starting...\n");
	
	cleanup(&program);
	return (0);
}