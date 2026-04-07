/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toruinoue <toruinoue@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 12:21:22 by torinoue          #+#    #+#             */
/*   Updated: 2025/11/04 04:29:27 by toruinoue        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_fd.h"

static int	convert_fd(const char cspdiux, va_list *ap, int fd)
{
	if (cspdiux == 'c')
		return (put_chr_nbr_fd(va_arg(*ap, int), 1, fd));
	if (cspdiux == 's')
		return (put_str_fd(va_arg(*ap, char *), fd));
	if (cspdiux == 'p')
		return (put_ptr_fd(va_arg(*ap, void *), fd));
	if (cspdiux == 'd' || cspdiux == 'i')
		return (put_xnbr_fd(va_arg(*ap, int), 10, 0, fd));
	if (cspdiux == 'u')
		return (put_xnbr_fd(va_arg(*ap, unsigned int), 10, 0, fd));
	if (cspdiux == 'x')
		return (put_xnbr_fd(va_arg(*ap, unsigned int), 16, 'a' - '0', fd));
	if (cspdiux == 'X')
		return (put_xnbr_fd(va_arg(*ap, unsigned int), 16, 'A' - '0', fd));
	if (cspdiux == '%')
		return (put_chr_nbr_fd('%', 1, fd));
	return (-1);
}

static int	get_retrn_value(size_t size, int len)
{
	int	return_value;

	if (INT_MAX >= size && len >= 0)
		return_value = (int)size;
	else
		return_value = -1;
	return (return_value);
}

int	ft_printf_fd(const int fd, const char *format, ...)
{
	va_list	ap;
	size_t	size;
	int		len;

	size = 0;
	va_start(ap, format);
	while (*format)
	{
		if (*format == '%')
		{
			len = convert_fd(*(format + 1), &ap, fd);
			format += 2;
		}
		else
		{
			len = put_chr_nbr_fd(*format, 1, fd);
			format ++;
		}
		if (len == -1)
			break ;
		size += len;
	}
	va_end(ap);
	return (get_retrn_value(size, len));
}

// return (  (i <= INT_MAX ) * i  - ( INT_MAX < i ) );
// if ( int_max >= i)
// 	i
// else -1
//return (size * !(INT_MAX < size || len < 0) - (INT_MAX < size || len < 0));
//git clone https://github.com/Tripouille/printfTester.git
//git clone https://github.com/paulo-santana/ft_printf_tester.git
//
//gcc main.c libftprintf.a

/*
# include <limits.h>
#include <stdio.h>
int main(void)
{
	// int size1, size2;

	printf("%f \n" , 42);
	ft_printf("%f \n" , 42);

	// printf("mozi %%%");
	// ft_printf("mozi\n");
	// printf("char and string\n");
	// char c = 'c';
	// char *s1 = "42tokyo";
	// char *s2 = NULL;
	// size1 = ft_printf("%c %s %s\n", c, s1, s2);
	// size2 =    printf("%c %s %s\n", c, s1, s2);
	// printf("%d %d\n\n", size1, size2);

	// printf("pointer\n");
	// void *p1 = &c;
	// void *p2 = NULL;
	// size1 = ft_printf("%p %p\n", p1, p2);
	// size2 =    printf("%p %p\n", p1, p2);
	// printf("%d %d\n\n", size1, size2);

	// printf("integer and hex\n");
	// int a = 16;
	// size1 = ft_printf("%d %i %u %x %X\n", a, a, a, a, a);
	// size2 =    printf("%d %i %u %x %X\n", a, a, a, a, a);
	// printf("%d %d\n\n", size1, size2);

	// a = -16;
	// size1 = ft_printf("%d %i %u %x %X\n", a, a, a, a, a);
	// size2 =    printf("%d %i %u %x %X\n", a, a, a, a, a);
	// printf("%d %d\n\n", size1, size2);

	// a = 0;
	// size1 = ft_printf("%d %i %u %x %X\n", a, a, a, a, a);
	// size2 =    printf("%d %i %u %x %X\n", a, a, a, a, a);
	// printf("%d %d\n\n", size1, size2);

	// a = INT_MAX;
	// size1 = ft_printf("%d %i %u %x %X\n", a, a, a, a, a);
	// size2 =    printf("%d %i %u %x %X\n", a, a, a, a, a);
	// printf("%d %d\n\n", size1, size2);

	// a = INT_MIN;
	// size1 = ft_printf("%d %i %u %x %X\n", a, a, a, a, a);
	// size2 =    printf("%d %i %u %x %X\n", a, a, a, a, a);
	// printf("%d %d\n\n", size1, size2);

	// printf("escape sequence\n");
	// size1 = ft_printf("%% %w %"); printf("\n");
	// size2 =    printf("%% %w %"); printf("\n");
	// printf("%d %d\n\n", size1, size2);

	// //size1 = ft_printf("%#10x"); printf("\n");
	// //size2 =    printf("%#10x"); printf("\n");
	// //printf("%d %d\n\n", size1, size2);

	// //size1 = ft_printf("%#10.4x"); printf("\n");
	// //size2 =    printf("%#10.4x"); printf("\n");
	// //printf("%d %d\n\n", size1, size2);

	system("leaks -q a.out");

	return (0);
}

*/