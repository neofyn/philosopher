/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyudris <fyudris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 09:28:00 by fyudris           #+#    #+#             */
/*   Updated: 2025/08/12 00:30:49 by fyudris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @brief Converts a string to an integer.
 * @param str The string to convert.
 * @return The converted integer, or -1 if the string is invalid.
 *
 * This function only handles positive integers and does not account for
 * overflow, which is acceptable for the project's constraints. It returns
 * -1 if a non-digit character is found.
 */
int	ft_atoi(const char *str)
{
	int	i;
	int	result;

	i = 0;
	result = 0;
	while (str[i] != '\0')
	{
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result);
}
