/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 15:19:29 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/03 09:38:34 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gnl/get_next_line.h"

/* ************************************************************************** */
int
	get_next_line(int fd, char** line);
int
	read_file_until_nl(t_str** str, int fd);
int
	malloc_next_line(t_str** str, char** line);
int
	write_next_line(t_str** str, char** line);
static int
	free_all(t_fd** list, int fd, char* buf);

/* ************************************************************************** */
// ファイルディスクリプタから1行読み込む
int
	get_next_line(int fd, char** line)
{
	static t_fd*	list = NULL;
	t_fd*			current;
	int				read_rem;
	char*			buffer;
	int				r;

	current = find_fd(&list, fd, &read_rem);
	if (!current) {
		return (free_all(&list, -1, NULL) | -1);
	}
	buffer = NULL;
	if (!read_rem && current->str) {
		read_rem = !find_nl(current->str, NULL);
	}
	if (read_rem) {
		r = read_file_until_nl(&current->str, fd);
		if (r < 0) {
			return (free_all(&list, (r == -1) ? -1 : fd, NULL) | -1);
		}
	}
	if (!malloc_next_line(&current->str, line)) {
		return (free_all(&list, -1, NULL) | -1);
	}
	read_rem = write_next_line(&current->str, line);
	if (!read_rem) {
		buffer = (char*)malloc(sizeof(*buffer) * (BUFFER_SIZE + 1));
		if (!buffer) {
			return (free_all(&list, -1, NULL));
		}
		r = read_file(&current->str, buffer, fd);
		if (free_all(NULL, -1, buffer) || r < 0) {
			return (free_all(&list, fd, NULL) | -1);
		}
	}
	if (r > 0 || read_rem) {
		return (1);
	}
	return (free_all(&list, fd, NULL));
}

/* ************************************************************************** */
// 改行が見つかるまでファイルから読み込みを続ける
int
	read_file_until_nl(t_str** str, int fd)
{
	char*	buffer;
	int		r;

	buffer = (char*)malloc(sizeof(*buffer) * (BUFFER_SIZE + 1));
	if (!buffer) {
		return (-1);
	}
	while ((r = read_file(str, buffer, fd)) > 0) {
		if (find_nl(NULL, buffer)) {
			break ;
		}
	}
	free(buffer);
	if (r < 0) {
		return (-2);
	}
	return (1);
}

/* ************************************************************************** */
// 1行分の文字列を格納するために必要なメモリを確保する
int
	malloc_next_line(t_str** str, char** line)
{
	t_str*	first;
	int		i;
	int		j;
	char*	buffer;

	first = *str;
	j = 0;
	while (*str) {
		i = 0;
		while ((*str)->content[i] && (*str)->content[i] != '\n') {
			i++;
			j++;
		}
		if ((*str)->content[i] == '\n') {
			break ;
		}
		*str = (*str)->next;
	}
	*str = first;
	buffer = (char*)malloc(sizeof(*buffer) * (j + 1));
	if (!buffer) {
		return (0);
	}
	*line = buffer;
	(*line)[j] = 0;
	return (1);
}

/* ************************************************************************** */
// 確保したメモリにバッファから1行分の文字列を書き込む
int
	write_next_line(t_str** str, char** line)
{
	int		idx0;
	int		idx1;
	int		remaining;
	t_str*	next;

	idx1 = 0;
	remaining = 0;
	while (*str) {
		idx0 = 0;
		while ((*str)->content[idx0] && (*str)->content[idx0] != '\n') {
			(*line)[idx1++] = (*str)->content[idx0++];
		}
		if ((*str)->content[idx0++] == '\n') {
			remaining = 1;
			idx1 = 0;
			while ((*str)->content[idx0]) {
				(*str)->content[idx1++] = (*str)->content[idx0++];
			}
			(*str)->content[idx1] = 0;
			break ;
		}
		next = (*str)->next;
		free((*str)->content);
		free(*str);
		*str = next;
	}
	return (remaining);
}

/* ************************************************************************** */
// リストやバッファのメモリを解放する
static int
	free_all(t_fd** list, int fd, char* buf)
{
	t_fd*	first;
	t_fd*	lt0;
	t_fd*	lt1;

	first = (list) ? *list : NULL;
	lt0 = NULL;
	while (list && *list) {
		lt1 = (*list)->next;
		if (fd < 0 || (*list)->fd == fd) {
			if (first == (*list)) {
				first = lt1;
			}
			str_clear(&(*list)->str);
			free((*list));
			if (lt0) {
				lt0->next = lt1;
			}
		}
		lt0 = (*list);
		(*list) = lt1;
	}
	if (list) {
		*list = first;
	}
	if (buf) {
		free(buf);
	}
	return (0);
}
