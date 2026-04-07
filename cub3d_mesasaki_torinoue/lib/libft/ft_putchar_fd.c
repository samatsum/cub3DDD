/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toruinoue <toruinoue@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 12:21:22 by torinoue          #+#    #+#             */
/*   Updated: 2025/10/25 22:37:42 by toruinoue        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// int	ft_putchar_fd(char c, int fd)
int	ft_putchar_fd(unsigned char c, int fd)
{
	return (write(fd, &c, 1));
}
