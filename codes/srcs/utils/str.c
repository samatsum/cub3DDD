/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 12:09:03 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/03 09:57:01 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/utils.h"

/* ************************************************************************** */
int
	str_length(t_str* str);
t_str*
	str_add_back(t_str** str, char* content);
t_str*
	str_last(t_str* str);
int
	str_clear(t_str** list);

/* ************************************************************************** */
// 文字列リストの要素数を返す
int
	str_length(t_str* str)
{
	int	i;

	i = 0;
	while (str) {
		str = str->next;
		i++;
	}
	return (i);
}

/* ************************************************************************** */
// 文字列リストの末尾に新しい要素を追加する
t_str*
	str_add_back(t_str** str, char* content)
{
	t_str*	first;
	t_str*	new;

	if (!content) {
		return (NULL);
	}
	new = (t_str*)malloc(sizeof(*new));
	if (!new) {
		return (0);
	}
	new->content = content;
	new->next = NULL;
	if (!*str) {
		*str = new;
	} else {
		first = *str;
		while ((*str)->next) {
			*str = (*str)->next;
		}
		(*str)->next = new;
		*str = first;
	}
	return (new);
}

/* ************************************************************************** */
// 文字列リストの最後の要素を返す
t_str*
	str_last(t_str* str)
{
	if (!str) {
		return (NULL);
	}
	while (str->next) {
		str = str->next;
	}
	return (str);
}

/* ************************************************************************** */
// 文字列リストの全ての要素のメモリを解放する
int
	str_clear(t_str** list)
{
	t_str*	tmp;

	while (*list) {
		tmp = (*list)->next;
		free((*list)->content);
		free(*list);
		(*list) = tmp;
	}
	return (0);
}
