/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyudris <fyudris@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 10:12:38 by fyudris           #+#    #+#             */
/*   Updated: 2025/11/28 11:49:08 by fyudris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/**
 * @brief Prints an error message to STDERR.
 * @param str The message to print.
 * @return 1 (Exit failure code).
 */
int	error_exit(char *str)
{
	printf("Error: %s\n", str);
	return (1);
}

/**
 * @brief Converts a string to a long integer.
 * Checks for validity (only digits, no signs allowed here for simplicity
 * as inputs must be positive).
 * @param str The string to convert.
 * @return The converted long, or -1 if invalid/overflow.
 */
long	ft_atol(const char *str)
{
	long	num;
	int		i;

	num = 0;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-')
		return (-1);
	if (str[i] == '+')
		i++;
	if (!str[i])
		return (-1);
	while (str[i] >= '0' && str[i] <= '9')
	{
		num = (num * 10) + (str[i] - '0');
		if (num > INT_MAX)
			return (-1);
		i++;
	}
	if (str[i] != '\0')
		return (-1);
	return (num);
}

/**
 * @brief Thread-safe print function.
 * FIX: Now locks BOTH write_lock and sim_lock to prevent data race.
 * @param philo The philosopher struct.
 * @param str The message to print.
 */
void	write_status(t_philo *philo, char *str)
{
	// 1. Lock Write (reserve stdout)
	pthread_mutex_lock(&philo->table->write_lock);

	// 2. Lock Sim (protect the read of sim_running)
	pthread_mutex_lock(&philo->table->sim_lock);
	
	if (philo->table->sim_running)
	{
		printf("%ld %d %s\n", get_time() - philo->table->start_time, \
			philo->id, str);
	}
	
	// 3. Unlock Sim
	pthread_mutex_unlock(&philo->table->sim_lock);

	// 4. Unlock Write
	pthread_mutex_unlock(&philo->table->write_lock);
}

/**
 * @brief Validates and parses command line arguments into the table struct.
 * @param table The main data structure.
 * @param argv Command line arguments.
 * @return 0 on success, 1 on failure.
 */
int	parse_input(t_table *table, char **argv)
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