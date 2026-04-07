/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_print.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toruinoue <toruinoue@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 00:00:00 by torinoue          #+#    #+#             */
/*   Updated: 2025/11/04 07:02:25 by toruinoue        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static int		get_radian_digit(const double radian, \
	int int_part, int multiplier);
static void		print_radian_part_fd(const int fd, const double radian);
static void		print_degree_part_fd(const int fd, const double degree);

/*
** Formats and prints a radian value and degree value (up to 360 degrees)
** to specified file descriptor
** Format: radian (1-digit integer part, decimal point, 1 decimal)
**         degree (3-digit integer part, decimal point, 1 decimal)
** Example: 0.123456 -> "rad: 0.1 deg: 000.7", 6.283185 -> "rad: 6.3 deg: 360.0"
**
** @param fd: File descriptor to write to
** @param radian: The radian value to format and 
** print (automatically normalized to 0-2π)
*/
void	print_angle_formatted_fd(const int fd, const double radian)
{
	double	normalized_radian;

	normalized_radian = normalize_angle(radian);
	print_radian_part_fd(fd, normalized_radian);
	ft_printf_fd(fd, " ");
	print_degree_part_fd(fd, radian_to_degree(normalized_radian));
}

/*
** Formats and prints a double value to specified file descriptor
** Format: If integer part >= 3 digits, print as integer
**         Otherwise, print with 6 decimal places
** Example: 0.002604 -> "0.002604", 7.000000 -> "7", 123.456 -> "123"
**
** @param fd: File descriptor to write to
** @param value: The double value to format and print
*/
void	print_double_formatted_fd(const int fd, double value)
{
	long	int_part;
	long	frac_part;
	int		digits[6];
	int		i;

	if (value == 0.0)
		return (ft_printf_fd(fd, "0.000000"), (void)0);
	value *= (value >= 0) - (0 < write(fd, "-", (value < 0)));
	int_part = (long)value;
	if (int_part > 99)
		return (ft_printf_fd(fd, "%d", (int)int_part), (void)0);
	frac_part = (long)((value - int_part) * 1000000.0 + 0.5);
	ft_printf_fd(fd, "%d.", (int)(int_part + \
		(frac_part >= 1000000)));
	frac_part *= (frac_part < 1000000);
	i = 5;
	while (i >= 0)
	{
		digits[i--] = frac_part % 10;
		frac_part /= 10;
	}
	while (++i < 6)
		ft_printf_fd(fd, "%d", digits[i]);
}

static int	get_radian_digit(const double radian, int int_part, int multiplier)
{
	int	digit;

	digit = (int)((radian - int_part) * multiplier) % 10;
	if (digit < 0)
		digit = 0;
	if (digit > 9)
		digit = 9;
	return (digit);
}

static void	print_radian_part_fd(const int fd, const double radian)
{
	int	int_part;

	int_part = (int)radian;
	ft_printf_fd(fd, "rad: %d.%d%d%d%d", int_part,
		get_radian_digit(radian, int_part, 10),
		get_radian_digit(radian, int_part, 100),
		get_radian_digit(radian, int_part, 1000),
		get_radian_digit(radian, int_part, 10000));
}

static void	print_degree_part_fd(const int fd, const double degree)
{
	int	deg_int_part;
	int	deg_dec_part;

	deg_int_part = (int)degree;
	deg_dec_part = (int)((degree - deg_int_part) * 10);
	if (deg_dec_part < 0)
		deg_dec_part = 0;
	if (deg_dec_part > 9)
		deg_dec_part = 9;
	if (deg_int_part < 10)
		ft_printf_fd(fd, "deg: 00%d.", deg_int_part);
	else if (deg_int_part < 100)
		ft_printf_fd(fd, "deg: 0%d.", deg_int_part);
	else
		ft_printf_fd(fd, "deg: %d.", deg_int_part);
	ft_printf_fd(fd, "%d", deg_dec_part);
}
