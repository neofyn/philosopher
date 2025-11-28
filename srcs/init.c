/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyudris <fyudris@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 09:45:47 by fyudris           #+#    #+#             */
/*   Updated: 2025/11/28 09:12:40 by fyudris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/**
 * @brief Validates and parses command line arguments into the table struct.
 * @param table The main data structure.
 * @param argv Command line arguments.
 * @return 0 on success, 1 on failure.
 */
static int	parse_input(t_table *table, char **argv)
{
	table->philo_nbr = ft_atol(argv[1]);
	table->time_die = ft_atol(argv[2]);
	table->time_eat = ft_atol(argv[3]);
	table->time_sleep = ft_atol(argv[4]);
	if (argv[5])
		table->must_eat_count = ft_atol(argv[5]);
	else
		table->must_eat_count = -1;
	if (table->philo_nbr < 1 || table->philo_nbr > 200
			|| table->time_die < 0 || table->time_eat < 0
			|| table-> time_sleep < 0
			|| (argv[5] && table->must_eat_count < 0))
			return (error_exit("Invalid arguments (Must be positive int)."));
	return (0);
}

/**
 * @brief Allocates memory for the philosophers and forks arrays.
 * @param table The main data structure.
 * @return 0 on success, 1 on failure.
 */
static int	alloc_data(t_table *table)
{
	table->philos = malloc(sizeof(t_philo *) * table->philo_nbr);
	if (!table->philos)
		return (error_exit("Malloc failed for philos."));
	table->forks = malloc(sizeof(pthread_mutex_t) * table->philo_nbr);
	if (!table->forks)
	{
		free(table->philos);
		return (error_exit("Mallof failed for forks."));
	}
	return (0);
}

/**
 * @brief Initialize the main table, memory, and mutexes.
 * @param table Pointer to the table struct.
 * @param argv Command line arguments.
 * @return 0 on success, 1 on failure.
 */
int	init_data(t_table *table, char **argv)
{
	int	i;

	if (parse_input(table, argv))
		return (1);
	if (alloc_data(table))
		return (1);
	if (pthread_mutex_init(&table->write_lock, NULL) != 0
		|| pthread_mutex_init(&table->sim_lock, NULL) != 0)
		return (error_exit("Mutex init failed."));
	i = 0;
	while (i < table->philo_nbr)
	{
		if (pthread_mutex_init(&table->forks[i], NULL) != 0)
			return (error_exit("Fork mutex init failed."));
		i++;
	}
	table->sim_running = true;
	return (0);
}
