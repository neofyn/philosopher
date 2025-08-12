/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyudris <fyudris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 00:49:11 by fyudris           #+#    #+#             */
/*   Updated: 2025/08/12 16:55:05 by fyudris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

// A helper function to print test results
static void	check(int success, const char *test_name)
{
	if (success)
		printf("  [✅] PASS: %s\n", test_name);
	else
		printf("  [❌] FAIL: %s\n", test_name);
}

// --- PARSING TESTS ---
static void	test_parsing(void)
{
	t_program	p;

	printf("--- Running Parsing Tests ---\n");

	// Test 1: Valid 5 arguments
	memset(&p, 0, sizeof(t_program));
	char *argv1[] = {"./philo", "5", "800", "200", "200"};
	check(init_program(&p, 5, argv1) == 0 && p.num_of_philos == 5, "Valid 5 args");
	cleanup(&p);

	// Test 2: Valid 6 arguments
	memset(&p, 0, sizeof(t_program));
	char *argv2[] = {"./philo", "4", "410", "200", "200", "7"};
	check(init_program(&p, 6, argv2) == 0 && p.num_of_meals == 7, "Valid 6 args");
	cleanup(&p);

	// Test 3: Invalid - 0 philosophers
	memset(&p, 0, sizeof(t_program));
	char *argv3[] = {"./philo", "0", "800", "200", "200"};
	check(init_program(&p, 5, argv3) != 0, "Invalid: 0 philosophers");
	cleanup(&p);

	// Test 4: Invalid - non-numeric argument
	memset(&p, 0, sizeof(t_program));
	char *argv4[] = {"./philo", "5", "800", "abc", "200"};
	check(init_program(&p, 5, argv4) != 0, "Invalid: Non-numeric arg");
	cleanup(&p);
}

// --- LOGGING TESTS ---
static void	test_logging(void)
{
	t_program	program;
	t_philo		philo;

	printf("\n--- Running Logging Tests ---\n");
	printf("Visually inspect the output below for the correct format:\n");
	printf("Expected format: <timestamp> <philo_id> <message>\n\n");

	// Setup a dummy program and philosopher for the test
	memset(&program, 0, sizeof(t_program));
	program.start_time = get_time();
	pthread_mutex_init(&program.print_lock, NULL);
	pthread_mutex_init(&program.death_lock, NULL);

	philo.id = 1;
	philo.program = &program;

	print_status(&philo, "has taken a fork");
	usleep(1000);
	print_status(&philo, "is eating");
	usleep(2000);
	print_status(&philo, "is sleeping");
	usleep(3000);
	print_status(&philo, "is thinking");
	pthread_mutex_destroy(&program.print_lock);
	pthread_mutex_destroy(&program.death_lock);
}

/**
 * @brief Main function to run all test suites.
 */
void	run_tests(void)
{
	test_parsing();
	test_logging();
}