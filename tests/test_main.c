/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyudris <fyudris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 09:46:31 by fyudris           #+#    #+#             */
/*   Updated: 2025/08/12 00:59:31 by fyudris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	run_tests(void);

/**
 * @brief The main entry point for the testing executable.
 * @return Returns 0 upon successful completion of tests.
 *
 * This function's only responsibility is to call the main test runner
 * function. It allows for a completely separate build target for testing.
 */
int	main(void)
{
	run_tests();
	return (0);
}