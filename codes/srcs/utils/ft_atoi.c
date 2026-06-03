/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by samatsum          #+#    #+#             */
/*   Updated: 2026/06/03 09:50:36 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/utils.h"

/* ************************************************************************** */
int
	ft_atoi(char const* str);

/* ************************************************************************** */
// 文字列を整数に変換する
int
	ft_atoi(char const* str)
{
	int	i;
	int	is_neg;
	int	res;

	if (!str) {
		return (0);
	}
	i = 0;
	while (str[i] == '\t' || str[i] == '\n' || str[i] == '\v' || str[i] == '\f' || str[i] == '\r' || str[i] == ' ') {
		i++;
	}
	is_neg = 1;
	if (str[i] == '-') {
		is_neg = -1;
	}
	if (is_neg == -1 || str[i] == '+') {
		i++;
	}
	res = 0;
	while (str[i] >= '0' && str[i] <= '9') {
		res = (res * 10) + (str[i] - '0');
		i++;
	}
	return (res * is_neg);
}
