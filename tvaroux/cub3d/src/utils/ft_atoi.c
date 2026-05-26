/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 13:27:15 by hyamamot          #+#    #+#             */
/*   Updated: 2026/02/20 13:27:15 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

int	check_sign(const char **nptr)
{
	int	is_negative;

	is_negative = 0;
	if (**nptr == '-' || **nptr == '+')
	{
		if (**nptr == '-')
			is_negative = 1;
		(*nptr)++;
	}
	return (is_negative);
}

int	ft_atoi(const char *nptr)
{
	int			is_negative;
	int			result;

	while (ft_isspace(*nptr))
		nptr++;
	is_negative = check_sign(&nptr);
	result = 0;
	while (ft_isdigit(*nptr))
	{
		result = result * 10 + (*nptr - '0');
		nptr++;
		if (is_negative && - result < INT_MIN)
			return (INT_MIN);
		else if (!is_negative && result > INT_MAX)
			return (INT_MAX);
	}
	if (is_negative)
		return (-result);
	else
		return (result);
}
