/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by samatsum          #+#    #+#             */
/*   Updated: 2026/06/03 09:53:00 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/utils.h"

/* ************************************************************************** */
char*
	ft_itoa(int n);
static int
	ft_abs(int nbr);
static void
	ft_strrev(char* str);

/* ************************************************************************** */
// 整数を文字列に変換する
char*
	ft_itoa(int n)
{
	char*	str;
	size_t	length;
	int		is_neg;
	int		i;

	is_neg = (n < 0);
	str = (char*)malloc((11 + is_neg) * sizeof(*str));
	if (!str) {
		return (NULL);
	}
	i = 0;
	while (i < (12 + is_neg)) {
		str[i++] = 0;
	}
	if (n == 0) {
		str[0] = '0';
	}
	length = 0;
	while (n != 0) {
		str[length++] = '0' + ft_abs(n % 10);
		n = (n / 10);
	}
	if (is_neg) {
		str[length] = '-';
	}
	ft_strrev(str);
	return (str);
}

/* ************************************************************************** */
// 数の絶対値を返す
static int
	ft_abs(int nbr)
{
	if (nbr < 0) {
		return (-nbr);
	} else {
		return (nbr);
	}
}

/* ************************************************************************** */
// 文字列を反転させる
static void
	ft_strrev(char* str)
{
	size_t	length;
	size_t	i;
	char	tmp;

	length = ft_strlen(str);
	i = 0;
	while (i < length / 2) {
		tmp = str[i];
		str[i] = str[length - i - 1];
		str[length - i - 1] = tmp;
		i++;
	}
}
