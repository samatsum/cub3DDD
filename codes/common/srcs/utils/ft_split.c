/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by samatsum          #+#    #+#             */
/*   Updated: 2026/06/03 09:54:44 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/utils.h"

/* ************************************************************************** */
t_str*
	ft_split(char const* org, char sep);

/* ************************************************************************** */
// 指定された区切り文字で文字列を分割し、リストにして返す
t_str*
	ft_split(char const* org, char sep)
{
	t_str*	str;
	int		i;
	int		start;

	start = 0;
	i = 0;
	str = NULL;
	while (org[i]) {
		if (org[i] == sep) {
			if (i - start > 0 && !str_add_back(&str, ft_substr(org, start, i - start))) {
				return (PTR_CAST(str_clear(&str)));
			}
			start = ++i;
		} else {
			i++;
		}
	}
	if (i - start > 0 && !str_add_back(&str, ft_substr(org, start, i - start))) {
		return (PTR_CAST(str_clear(&str)));
	}
	return (str);
}
