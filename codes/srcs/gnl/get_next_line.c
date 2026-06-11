/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 15:19:29 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/11 22:04:56 by samatsum         ###   ########.fr       */
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
	gnl_clear(t_fd** list, int fd, char** line);
static int
	free_all(t_fd** list, int fd, char* buf);

/* ************************************************************************** */
// fdから1行読み込む（成功:1 / 終端:0 / エラー:負。終端・エラー時は *line を NULL 化する）
int
	get_next_line(int fd, char** line)
{
	static t_fd*	list = NULL;
	t_fd*			current;
	int				is_new;
	int				r;

	if (fd < 0 || !line || BUFFER_SIZE <= 0) {
		return (gnl_clear(&list, fd, line));
	}
	*line = NULL;
	current = find_fd(&list, fd, &is_new);
	if (!current) {
		return (gnl_clear(&list, -1, line));
	}
	if (is_new || !current->str || !find_nl(current->str, NULL)) {
		r = read_file_until_nl(&current->str, fd);
		if (r < 0) {
			return (gnl_clear(&list, (r == -1) ? -1 : fd, line));
		}
	}
	if (!malloc_next_line(&current->str, line)) {
		return (gnl_clear(&list, -1, line));
	}
	r = write_next_line(&current->str, line);
	if (r == 0 && ft_strlen(*line) == 0) {
		free(*line);
		*line = NULL;
		free_all(&list, fd, NULL);
		return (0);
	}
	return (1);
}

/* ************************************************************************** */
// 改行が見つかるまでファイルから読み込みを続ける（成功:1 / 自身のmalloc失敗:-1 / 読込失敗:-2）
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
// 確保したメモリにバッファから1行分の文字列を書き込む（改行を消費したら 1 を返す）
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
// 内部リストとバッファを解放し、*line を NULL 化して -1 を返す（エラー処理の共通化）
static int
	gnl_clear(t_fd** list, int fd, char** line)
{
	free_all(list, fd, NULL);
	if (line) {
		*line = NULL;
	}
	return (-1);
}

/* ************************************************************************** */
// 指定fd(負なら全fd)のリストノードとバッファを解放する（解放済みノードを再参照しない）
static int
	free_all(t_fd** list, int fd, char* buf)
{
	t_fd*	cur;
	t_fd*	prev;
	t_fd*	next;

	prev = NULL;
	if (list) {
		cur = *list;
		while (cur) {
			next = cur->next;
			if (fd < 0 || cur->fd == fd) {
				if (prev) {
					prev->next = next;
				} else {
					*list = next;
				}
				str_clear(&cur->str);
				free(cur);
			} else {
				prev = cur;
			}
			cur = next;
		}
	}
	if (buf) {
		free(buf);
	}
	return (0);
}
