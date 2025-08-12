/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyudris <fyudris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 09:30:17 by fyudris           #+#    #+#             */
/*   Updated: 2025/08/12 01:03:06 by fyudris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/philo.h"

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
		cleanup(&program);
		return (1);
	}
	
	// TODO: THREAD CREATION AND MONITORING WILL GO HERE ----
	printf("Initialization successful. Simulation starting...\n");
	
	cleanup(&program);
	return (0);
}