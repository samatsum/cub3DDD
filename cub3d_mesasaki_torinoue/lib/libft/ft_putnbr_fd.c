/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toruinoue <toruinoue@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 12:21:22 by torinoue          #+#    #+#             */
/*   Updated: 2025/10/25 22:56:42 by toruinoue        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbr_fd(int n, int fd)
{
	int	divisor;
	int	tmp_n;
	int	quotient;
	int	length;

	length = 0;
	if (n == 0)
		return (ft_putchar_fd('0', fd));
	if (n < 0)
		length = ft_putchar_fd('-', fd);
	tmp_n = n / 10;
	divisor = 1;
	while (tmp_n)
	{
		divisor *= 10;
		tmp_n /= 10;
	}
	while (divisor)
	{
		quotient = ((n >= 0) - (n < 0)) * (n / divisor) + '0';
		length += ft_putchar_fd(quotient, fd);
		n = n % divisor;
		divisor /= 10;
	}
	return (length);
}

//#include	<stdio.h>
// int	ft_putnbr_fd(int n, int fd)
// {
// 	int	bytes;
// 
// 	bytes = 0;
// 	if (n < 0)
// 	{
// 		if (n == -2147483648)
// 			return (ft_dprintstr("-2147483648", fd));
// 		bytes += ft_dprintchr('-', fd);
// 		n *= -1;
// 	}
// 	if (n >= 10)
// 		bytes += ft_dprintnbr(n / 10, fd);
// 	bytes += ft_dprintchr(n % 10 + '0', fd);
// 	return (bytes);
// }
// 
//int	main(void)
//{
//		ft_putnbr_fd(-2147483648, 1);
//		printf("\n");
//		ft_putnbr_fd( 2147483647, 1);
//	return (0);
//}