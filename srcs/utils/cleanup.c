/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyudris <fyudris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 10:02:24 by fyudris           #+#    #+#             */
/*   Updated: 2025/08/12 01:02:45 by fyudris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/philo.h"

/**
 * @brief Cleans up all allocated memory and destroys mutexes.
 * @param program The main program structure.
 *
 * This function is essential for preventing memory leaks and ensuring
 * that all mutex resources are properly released. It checks if pointers
 * are non-NULL before freeing to avoid errors on partial initialization.
 */
void	cleanup(t_program *program)
{
	int	i;

	i = 0;
	if (program->forks)
	{
		while (i < program->num_of_philos)
		{
			pthread_mutex_destroy(&program->forks[i]);
			i++;
		}
	}
	pthread_mutex_destroy(&program->print_lock);
	pthread_mutex_destroy(&program->death_lock);
	if (program->philos)
		free(program->philos);
	if (program->forks)
		free(program->forks);
}