/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_fd.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toruinoue <toruinoue@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 20:27:02 by  torinoue         #+#    #+#             */
/*   Updated: 2025/11/04 04:29:26 by toruinoue        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_FD_H
# define FT_PRINTF_FD_H
# include <stddef.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdarg.h>
# include <limits.h>
# include <stdint.h>

int		ft_printf_fd(const int fd, const char *format, ...) \
		__attribute__((format(printf, 2, 3)));
size_t	ft_strlen_fd(const char *s);
int		put_str_fd(char *s, int fd);
int		put_chr_nbr_fd(unsigned char chr_code, int chr_flag, int fd);
int		put_xnbr_fd(long n, int base, int flag, int fd);
int		put_ptr_xnbr_fd(uintptr_t n, int base, int flag, int fd);
int		put_ptr_fd(void *ptr, int fd);

#endif