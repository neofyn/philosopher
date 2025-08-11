/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyudris <fyudris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 09:13:27 by fyudris           #+#    #+#             */
/*   Updated: 2025/08/12 00:20:14 by fyudris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>	// For threads and mutexes
# include <sys/time.h>	// For gettimeofday
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

/**
 * @brief Represents a single philosopher.
 *
 * @param id The unique identifier for the philosopher (1 to N).
 * @param thread_id The thread identifier returned by pthread_create.
 * @param left_fork Pointer to the mutex representing the left fork.
 * @param right_fork Pointer to the mutex representing the right fork.
 * @param last_meal_time The timestamp of the beginning of the last meal.
 * @param meals_eaten The number of meals this philosopher has eaten.
 * @param program A pointer to the main program structure for shared data
 * access.
 */
typedef struct s_philo
{
	int					id;
	pthread_t			thread_id;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	long				last_meal_time;
	int					meals_eaten;
	struct s_program	*program;
}	t_philo;

/**
 * @brief Holds all the program's simulation parameters and shared resources.
 *
 * @param num_of_philos The total number of philosophers.
 * @param time_to_die The time (in ms) a philosopher can go without eating.
 * @param time_to_eat The time (in ms) it takes to eat.
 * @param time_to_sleep The time (in ms) it takes to sleep.
 * @param num_of_meals The optional number of meals each philosopher must eat.
 * @param start_time The timestamp (in ms) when the simulation started.
 * @param philos An array of all philosopher structures.
 * @param forks An array of mutexes, one for each fork.
 * @param print_lock A mutex to prevent garbled output from printf.
 * @param death_lock A mutex to protect access to the `stop_simulation` flag.
 * @param stop_simulation A flag that is 0 initially and 1 if a philosopher
 * dies or all meals are eaten. This signals all threads to terminate.
 */
typedef struct s_program
{
	int				num_of_philos;
	long			time_to_die;
	long			time_to_eat;
	int				num_of_meals;
	long			start_time;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	death_lock;
	int				stop_simulation;
}	t_program;
#endif