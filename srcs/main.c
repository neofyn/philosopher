/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyudris <fyudris@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 09:30:17 by fyudris           #+#    #+#             */
/*   Updated: 2025/11/28 20:10:57 by fyudris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/**
 * Using ThreadSanitizer (TSan) for finding Data Races quickly.
 * 1. Compile with TSan flag:
 *    make re CFLAGS="-g -pthread -fsanitize=thread"
 * 2. Run the program. If you get a memory mapping error, disable ASLR:
 *    setarch $(uname -m) -R ./philo 4 410 200 200
 *
 * Using Helgrind (Valgrind) for finding Deadlocks (Lock Order Violations) and
 * misuse of Mutexes (e.g., unlocking a mutex you don't own).
 * 1. Compile WITHOUT the -fsanitize=thread flag.
 * 2. Run with Helgrind:
 *    valgrind --tool=helgrind ./philo 4 410 200 200
 */
/**
 * @brief Main entry point for the Dining Philosophers simulation.
 *
 * The program follows these steps:
 * 1.	Validates command-line arguments.
 * 2.	Initializes the main `t_table` struct, which includes parsing input,
 *   	allocating memory for philosophers and forks, and initializing all
 * 	 	mutexes.
 * 3.	Creates and starts a thread for each philosopher.
 * 4.	Creates and starts a monitor thread to check for deaths or if all
 *   	philosophers have eaten enough.
 * 5.	Waits for the monitor thread to complete. The monitor exits when the
 *   	simulation ends (due to a death or all meals being eaten).
 * 6.	Waits for all philosopher threads to join, ensuring they have finished
 *   	their routines and exited cleanly.
 * 7.	Cleans up all allocated memory and destroys all mutexes.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line argument strings.
 * @return 0 on successful execution, 1 on error.
 */
int	main(int argc, char **argv)
{
	t_table		table;
	pthread_t	monitor_thread;
	int			i;

	if (argc < 5 || argc > 6)
		return (error_exit("Wrong argument count."));

	// 1. Initialize Data
	if (init_data(&table, argv) != 0)
		return (1);

	// 2. Start Philosopher Threads
	if (start_simulation(&table) != 0)
		return (1);

	// 3. Start Monitor Thread
	if (pthread_create(&monitor_thread, NULL, monitor_routine, &table) != 0)
		return (error_exit("Monitor thread creation failed."));

	// 4. Wait for Monitor to finish (death or all ate)
	pthread_join(monitor_thread, NULL);

	// 5. Wait for Philosophers to finish
	i = 0;
	while (i < table.philo_nbr)
	{
		pthread_join(table.philos[i]->thread_id, NULL);
		i++;
	}

	// 6. Clean up
	cleanup(&table);
	return (0);
}
