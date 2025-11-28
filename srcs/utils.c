/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyudris <fyudris@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 10:12:38 by fyudris           #+#    #+#             */
/*   Updated: 2025/11/28 10:11:01 by fyudris          ###   ########.fr       */
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
 * Locks the write_mutex, checks if simulation is running, prints, then unlocks.
 * @param philo The philosopher struct.
 * @param str The message to print (e.g., "is eating").
 */
void	write_status(t_philo *philo, char *str)
{
	pthread_mutex_lock(&philo->table->write_lock);
	// We check sim_running so we don't print "died" after someone else died
	if (philo->table->sim_running)
	{
		printf("%ld %d %s\n", get_time() - philo->table->start_time,
			philo->id, str);
	}
	pthread_mutex_unlock(&philo->table->write_lock);
}
