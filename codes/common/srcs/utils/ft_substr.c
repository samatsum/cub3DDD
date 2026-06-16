/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 13:42:27 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/03 09:55:59 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/utils.h"

/* ************************************************************************** */
char*
	ft_substr(char const* s, int start, int len);

/* ************************************************************************** */
// 文字列の一部を切り出して新しい文字列を作成する
char*
	ft_substr(char const* s, int start, int len)
{
	char*	str;
	int		i;
	int		j;

	str = (char*)malloc(sizeof(*s) * (len + 1));
	if (!str) {
		return (NULL);
	}
	i = 0;
	j = 0;
	while (s[i]) {
		if (i >= start && j < len) {
			str[j] = s[i];
			j++;
		}
		i++;
	}
	str[j] = 0;
	return (str);
}
