/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyudris <fyudris@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 11:06:19 by fyudris           #+#    #+#             */
/*   Updated: 2025/11/28 20:20:50 by fyudris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/**
 * @brief Destroys the main simulation mutexes (write and sim_lock).
 *
 * @param table The main data structure.
 */
static void	destroy_table_mutexes(t_table *table)
{
	pthread_mutex_destroy(&table->write_lock);
	pthread_mutex_destroy(&table->sim_lock);
}

/**
 * @brief Frees the philosopher structures and their specific mutexes.
 *
 * Iterates through each philosopher, destroys their personal `meal_lock` mutex,
 * and then frees the memory allocated for the `t_philo` struct itself.
 *
 * @param table The main data structure.
 */
static void	free_philos(t_table *table)
{
	int	i;

	if (!table->philos)
		return ;
	i = 0;
	while (i < table->philo_nbr)
	{
		if (table->philos[i])
		{
			pthread_mutex_destroy(&table->philos[i]->meal_lock);
			free(table->philos[i]);
		}
		i++;
	}
	free(table->philos);
}

/**
 * @brief Frees the forks array and destroys their mutexes.
 *
 * Iterates through the forks array, destroying each fork's mutex before
 * freeing the array's memory.
 *
 * @param table The main data structure.
 */
static void	free_forks(t_table *table)
{
	int	i;

	if (!table->forks)
		return ;
	i = 0;
	while (i < table->philo_nbr)
	{
		pthread_mutex_destroy(&table->forks[i]);
		i++;
	}
	free(table->forks);
}

/**
 * @brief Main cleanup function.
 * Destroys all mutexes and frees all allocated memory.
 *
 * This function is the single entry point for deallocating all resources. It
 * calls helper functions to destroy all mutexes (forks, meal locks, table
 * locks) and free all dynamically allocated memory in a safe order.
 * @param table The main data structure.
 */
void	cleanup(t_table *table)
{
	free_forks(table);
	free_philos(table);
	destroy_table_mutexes(table);
}
