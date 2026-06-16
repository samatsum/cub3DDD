/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_endwith.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 17:32:01 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/03 09:52:25 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/utils.h"

/* ************************************************************************** */
int
	ft_endwith(char const* str, char const* end);

/* ************************************************************************** */
// 文字列が指定したサフィックスで終わっているかを判定する
int
	ft_endwith(char const* str, char const* end)
{
	int	length;
	int	end_length;

	length = ft_strlen(str);
	end_length = ft_strlen(end);
	if (end_length > length) {
		return (0);
	}
	return (!ft_strcmp(str + length - end_length, end));
}
