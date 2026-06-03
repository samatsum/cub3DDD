/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 18:21:51 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/03 09:54:19 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/utils.h"

/* ************************************************************************** */
void*
	ft_memmove(void* dst, const void* src, unsigned long len);

/* ************************************************************************** */
// メモリ領域をコピーする。領域が重なっていても正しくコピーされる
void*
	ft_memmove(void* dst, const void* src, unsigned long len)
{
	int	i;

	if (!dst || !src) {
		return (NULL);
	}
	if (dst > src) {
		i = (int)len - 1;
		while (i >= 0) {
			*(char*)(dst + i) = *(char*)(src + i);
			i--;
		}
	} else {
		i = 0;
		while (i < (int)len) {
			*(char*)(dst + i) = *(char*)(src + i);
			i++;
		}
	}
	return (dst);
}
