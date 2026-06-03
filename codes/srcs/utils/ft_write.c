/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_write.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:22:02 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/03 09:56:34 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/utils.h"

/* ************************************************************************** */
int
	ft_write_int(char* buf, int val, int start);
int
	ft_write_str(char* buf, char* str, int start);

/* ************************************************************************** */
// バッファに整数を文字列として書き込む
int
	ft_write_int(char* buf, int val, int start)
{
	int	length;
	int	tmp;

	length = 1;
	tmp = val;
	while (tmp > 9) {
		length++;
		tmp /= 10;
	}
	if (val == 0) {
		buf[start++] = '0';
	} else {
		tmp = length;
		while (val > 0) {
			buf[start + --tmp] = "0123456789"[val % 10];
			val /= 10;
		}
		start += length;
	}
	buf[start] = 0;
	return (start);
}

/* ************************************************************************** */
// バッファに文字列を書き込む
int
	ft_write_str(char* buf, char* str, int start)
{
	int	i;

	i = 0;
	while (str[i]) {
		buf[start++] = str[i++];
	}
	buf[start] = 0;
	return (start);
}
