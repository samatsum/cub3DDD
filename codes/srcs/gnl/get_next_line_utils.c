/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 16:00:46 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/03 09:38:51 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gnl/get_next_line.h"

/* ************************************************************************** */
t_fd*
	find_fd(t_fd** list, int fd, int* new);
int
	find_nl(t_str* str, char* sim_str);
int
	read_file(t_str** str, char* buffer, int fd);
int
	lst_clear(t_str** list);

/* ************************************************************************** */
// 指定されたファイルディスクリプタに対応するリストの要素を探す、または作成する
t_fd*
	find_fd(t_fd** list, int fd, int* new)
{
	t_fd*	ret;
	t_fd*	first;

	ret = NULL;
	first = *list;
	*new = 0;
	while (*list && !ret) {
		if ((*list)->fd == fd) {
			ret = *list;
		}
		*list = (*list)->next;
	}
	*list = first;
	if (!ret) {
		ret = (t_fd*)malloc(sizeof(*ret));
		if (!ret) {
			return (NULL);
		}
		ret->fd = fd;
		ret->next = *list;
		ret->str = NULL;
		*list = ret;
		*new = 1;
	}
	return (ret);
}

/* ************************************************************************** */
// リストまたは文字列内に改行が含まれているかを判定する
int
	find_nl(t_str* str, char* sim_str)
{
	int	i;

	if (!str) {
		i = 0;
		while (sim_str[i] && sim_str[i] != '\n') {
			i++;
		}
		if (sim_str[i] == '\n') {
			return (1);
		}
	} else {
		while (str) {
			i = 0;
			while (str->content[i] && str->content[i] != '\n') {
				i++;
			}
			if (str->content[i] == '\n') {
				return (1);
			}
			str = str->next;
		}
	}
	return (0);
}

/* ************************************************************************** */
// ファイルからバッファに読み込み、リストに追加する
int
	read_file(t_str** str, char* buffer, int fd)
{
	int		r;
	t_str*	new;
	t_str*	first;

	r = read(fd, buffer, BUFFER_SIZE);
	if (r > 0) {
		buffer[r] = 0;
		new = (t_str*)malloc(sizeof(*new));
		if (!new) {
			return (-2);
		}
		new->content = ft_strdup(buffer);
		if (!new->content) {
			free(new);
			return (-2);
		}
		new->next = NULL;
		if (!*str) {
			*str = new;
		} else {
			first = *str;
			while ((*str)->next) {
				(*str) = (*str)->next;
			}
			(*str)->next = new;
			*str = first;
		}
		return (1);
	}
	if (r < 0) {
		return (-1);
	}
	return (0);
}

/* ************************************************************************** */
// 文字列リストの全要素のメモリを解放する
int
	lst_clear(t_str** list)
{
	t_str*	tmp;

	while (*list) {
		tmp = (*list)->next;
		free((*list)->content);
		free(*list);
		(*list) = tmp;
	}
	*list = NULL;
	return (0);
}
