/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_cspdiux_fd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toruinoue <toruinoue@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 23:50:48 by toruinoue         #+#    #+#             */
/*   Updated: 2025/10/26 00:08:24 by toruinoue        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_fd.h"

int	put_str_fd(char *s, int fd)
{
	if (s == NULL)
		return (write(fd, "(null)", 6));
	return (write(fd, s, ft_strlen_fd(s)));
}

int	put_chr_nbr_fd(unsigned char chr_code, int chr_flag, int fd)
{
	if (chr_flag)
		return (write(fd, &chr_code, 1));
	if (chr_code == 0)
		return (0);
	return (write(fd, &chr_code, 1));
}

int	put_xnbr_fd(long n, int base, int flag, int fd)
{
	int	divisor;
	int	tmp_n;
	int	quot;
	int	len;

	if (n == 0)
		return (put_chr_nbr_fd('0', 0, fd));
	len = (n < 0) * put_chr_nbr_fd((n < 0) * '-', 0, fd);
	tmp_n = n / base;
	divisor = 1;
	while (tmp_n)
	{
		divisor *= base;
		tmp_n /= base;
	}
	while (divisor > 0 && len >= 0)
	{
		quot = ((n >= 0) - (n < 0)) * (n / divisor);
		if (put_chr_nbr_fd('0' + quot + (quot > 9) * (flag - 10), 0, fd) == -1)
			return (-1);
		len ++;
		n = n % divisor;
		divisor /= base;
	}
	return (len);
}

int	put_ptr_xnbr_fd(uintptr_t n, int base, int flag, int fd)
{
	unsigned long	divisor;
	unsigned long	tmp_n;
	unsigned long	quot;
	int				len;

	len = 0;
	if (n == 0)
		return (put_chr_nbr_fd('0', 0, fd));
	tmp_n = n / base;
	divisor = 1;
	while (tmp_n)
	{
		divisor *= base;
		tmp_n /= base;
	}
	while (divisor)
	{
		quot = (n / divisor);
		if (put_chr_nbr_fd('0' + quot + (quot > 9) * (flag - 10), 0, fd) == -1)
			return (-1);
		len ++;
		n = n % divisor;
		divisor /= base;
	}
	return (len);
}

int	put_ptr_fd(void *ptr, int fd)
{
	int	len;

	len = write(fd, "0x", 2);
	if (len == -1)
		return (-1);
	len += put_ptr_xnbr_fd((uintptr_t)ptr, 16, 'a' - '0', fd);
	if (len < 2)
		return (-1);
	return (len);
}

//norminette ft_printf_fd.c ft_strlen_fd.c put_cspdiux_fd.c ft_printf_fd.h