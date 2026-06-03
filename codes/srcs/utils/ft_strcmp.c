/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 18:42:21 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/03 09:55:22 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/utils.h"

/* ************************************************************************** */
int
	ft_strcmp(const char* s1, const char* s2);

/* ************************************************************************** */
// 二つの文字列を比較し、差分を返す
int
	ft_strcmp(const char* s1, const char* s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i]) {
		if ((unsigned char)s1[i] != (unsigned char)s2[i]) {
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		}
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
