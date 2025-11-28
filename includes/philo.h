/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyudris <fyudris@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 09:13:27 by fyudris           #+#    #+#             */
/*   Updated: 2025/11/28 11:04:42 by fyudris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>	// For threads and mutexes
# include <sys/time.h>	// For gettimeofday
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <stdbool.h>
# include <limits.h>

/*───────────────────────────────────────────────*/
/*                MAIN DATA STRUCTURE            */
/*───────────────────────────────────────────────*/

typedef struct s_philo	t_philo;

/**
 * @brief (Shared memory) Holds the rules and the shared resources (forks).
 */
typedef struct s_table
{
	long			philo_nbr;
	long			time_die;
	long			time_eat;
	long			time_sleep;
	long			must_eat_count;	//-1 if flag not present
	long			start_time;
	bool			sim_running;// Flag to stop simulation
	pthread_mutex_t	sim_lock;	// Protects the sim_running flag
	pthread_mutex_t	write_lock;	// Prevent scrambled print output
	pthread_mutex_t	*forks;		// Array of fork mutexes
	t_philo			**philos;	// Array of philosopher pointers
}	t_table;

/**
 * @brief (Thread Data) Holds data specific to one thread
 */
typedef struct s_philo
{
	int				id;
	long			meals_eaten;
	long			last_meal_time;
	pthread_t		thread_id;
	pthread_mutex_t	*fork_first;
	pthread_mutex_t	*fork_second;
	pthread_mutex_t	meal_lock;	// Protect last_meal_time variable
	t_table			*table;
}	t_philo;

/*───────────────────────────────────────────────*/
/*                FUNCTION PROTOTYPES            */
/*───────────────────────────────────────────────*/

/* init.c */
int	init_data(t_table *table, char **argv);
int	start_simulation(t_table *table);

/* utils.c */
int		error_exit(char *str);
long	ft_atol(const char *str);
void	write_status(t_philo *philo, char *str);

/* time.c */
long	get_time(void);
void	precise_usleep(long time_in_ms, t_table *table);

/* routine.c */
void	*philo_routine(void *ptr);

/* monitor.c */
void	*monitor_routine(void *ptr);

#endif