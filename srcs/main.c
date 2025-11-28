/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyudris <fyudris@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 09:30:17 by fyudris           #+#    #+#             */
/*   Updated: 2025/11/28 20:29:41 by fyudris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	main(int argc, char **argv)
{
	t_table		table;
	pthread_t	monitor_thread;
	int			i;

	if (argc < 5 || argc > 6)
		return (error_exit("Wrong argument count."));
	if (init_data(&table, argv) != 0)
		return (1);
	if (start_simulation(&table) != 0)
		return (1);
	if (pthread_create(&monitor_thread, NULL, monitor_routine, &table) != 0)
		return (error_exit("Monitor thread creation failed."));
	pthread_join(monitor_thread, NULL);
	i = 0;
	while (i < table.philo_nbr)
	{
		pthread_join(table.philos[i]->thread_id, NULL);
		i++;
	}
	cleanup(&table);
	return (0);
}
