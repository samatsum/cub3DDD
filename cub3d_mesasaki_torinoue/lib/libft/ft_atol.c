/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: torinoue <21091003@mst.meiji-u.ac.jp>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 12:12:07 by torinoue          #+#    #+#             */
/*   Updated: 2023/03/25 19:46:49 by torinoue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long	ft_atol(const char *str)
{
	int			sign;
	long		value;
	size_t		i;

	value = 0;
	i = 0;
	while (ft_strchr("\t\n\v\f\r ", str[i]) != NULL)
		i++;
	sign = (str[i] != '-') - (str[i] == '-');
	i += (str[i] == '-' || str[i] == '+');
	while (ft_isdigit(str[i]))
	{
		if ((value > LONG_MAX / 10)
			|| (value == (LONG_MAX / 10) && ((str[i] - '0') > (LONG_MAX % 10))))
			return ((sign == 1) * (LONG_MAX)
				+ (sign == -1) * (LONG_MIN));
		value = value * 10 + (str[i++] - '0');
	}
	return ((long)(value * sign));
}

/*
#include	<stdio.h>
int	main(int argc, char **argv)
{
	if (argc == 2)
		printf("%ld\n", ft_atol(argv[1]));
	return (0);
}
*/