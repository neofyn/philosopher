/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyudris <fyudris@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 09:45:47 by fyudris           #+#    #+#             */
/*   Updated: 2025/11/28 19:25:22 by fyudris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

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
		return (error_exit("Malloc failed for forks."));
	}
	return (0);
}

/**
 * @brief Assigns fork mutex pointers to a philosopher.
 * Implements the resource hierarchy solution to prevent deadlocks.
 * Logic:
 * 	Fork 1 is at index 'i' (Left)
 * 	Fork 2 is at index '(i + 1) % nbr' (Right, circular)
 * @param philo The philosopher struct.
 * @param forks The array of fork mutexes.
 * @param i The index of the philosopher.
 */
static void	assign_forks(t_philo *philo, pthread_mutex_t *forks, int i)
{
	int	philo_nbr;

	philo_nbr = philo->table->philo_nbr;
	// We swap the order for odd IDs (1, 3, 5...) to break circular wait
	if (philo->id % 2 == 0)
	{
		philo->fork_first = &forks[i];
		philo->fork_second = &forks[(i + 1) % philo_nbr];
	}
	else
	{
		philo->fork_first = &forks[(i + 1) % philo_nbr];
		philo->fork_second = &forks[i];
	}
}

/**
 * @brief Allocates and initializes individual philosopher structs.
 * @param table The main data structure.
 * @return 0 on success, 1 on failure.
 */
static int	init_philos(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->philo_nbr)
	{
		table->philos[i] = malloc(sizeof(t_philo));
		if (!table->philos[i])
			return (error_exit("Malloc failed for a philo."));
		table->philos[i]->id = i + 1; // ID starts at 1
		table->philos[i]->meals_eaten = 0;
		table->philos[i]->last_meal_time = 0; // Set properly when routine starts
		table->philos[i]->table = table;

		// Initialize the personal meal lock
		if (pthread_mutex_init(&table->philos[i]->meal_lock, NULL) != 0)
			return (error_exit("Meal mutex init failed."));

		assign_forks(table->philos[i], table->forks, i);
		i++;
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
	if (init_philos(table) != 0)
		return (1);

	table->sim_running = true;
	return (0);
}

/**
 * @brief Creates the philosopher threads.
 * @param table The main data structure.
 * @return 0 on success, 1 on failure.
 */
int	start_simulation(t_table *table)
{
	int	i;

	table->start_time = get_time();
	i = 0;
	while (i < table->philo_nbr)
	{
		// Set last meal time to start time before thread starts
		table->philos[i]->last_meal_time = table->start_time;
		if (pthread_create(&table->philos[i]->thread_id, NULL, \
			philo_routine, table->philos[i]) != 0)
			return (error_exit("Thread creation failed."));
		i++;
	}
	return (0);
}


