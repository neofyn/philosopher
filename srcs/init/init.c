/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyudris <fyudris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 09:29:44 by fyudris           #+#    #+#             */
/*   Updated: 2025/08/12 00:30:54 by fyudris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/philo.h"

/**
 * @brief Initializes the mutexes for forks and locks.
 * @param program The main program structure.
 * @return Returns 0 on success, 1 on failure.
 */
static int	init_mutexes(t_program *program)
{
	int	i;

	i = 0;
	while (i < program->num_of_philos)
	{
		if (pthread_mutex_init(&program->forks[i], NULL) != 0)
			return (1); // Mutex init failed
		i++;
	}
	if (pthread_mutex_init(&program->print_lock, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&program->death_lock, NULL) != 0)
		return (1);
	return (0);
}

/**
 * @brief Initializes the philosopher structures.
 * @param program The main program structure.
 */
static void	init_philos(t_program *program)
{
	int	i;

	i = 0;
	while (i < program->num_of_philos)
	{
		program->philos[i].id = i + 1;
		program->philos[i].meals_eaten = 0;
		program->philos[i].last_meal_time = program->start_time;
		program->philos[i].program = program;
		// Assign forks
		program->philos[i].left_fork = &program->forks[i];
		// The last philosopher's right fork is the first one (index 0)
		if (i == program->num_of_philos - 1)
			program->philos[i].right_fork = &program->forks[0];
		else
			program->philos[i].right_fork = &program->forks[i + 1];
		i++;
	}
}

/**
 * @brief Initializes the main program structure with arguments.
 * @param program The main program structure.
 * @param argc The argument count.
 * @param argv The argument values.
 * @return Returns 0 on success, 1 on failure (e.g., bad input).
 */
int	init_program(t_program *program, int argc, char **argv)
{
	program->num_of_philos = ft_atoi(argv[1]);
	program->time_to_die = ft_atoi(argv[2]);
	program->time_to_eat = ft_atoi(argv[3]);
	program->time_to_sleep = ft_atoi(argv[4]);
	if (program->num_of_philos <= 0 || program->time_to_die < 0
		|| program->time_to_eat < 0 || program->time_to_sleep < 0)
		return (1);
	if (argc == 6)
	{
		program->num_of_meals = ft_atoi(argv[5]);
		if (program->num_of_meals <= 0)
			return (1);
	}
	else
		program->num_of_meals = -1; // Indicates no meal limit
	program->philos = malloc(sizeof(t_philo) * program->num_of_philos);
	program->forks = malloc(sizeof(pthread_mutex_t) * program->num_of_philos);
	if (!program->philos || !program->forks || init_mutexes(program) != 0)
		return (1); // Malloc or mutex init failed
	program->start_time = get_time();
	init_philos(program);
	return (0);
}