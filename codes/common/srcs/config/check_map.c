/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 07:32:33 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/17 14:32:08 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config/config.h"

/* ************************************************************************** */
int
	count_check_columns(char const* line);
int
	check_top_bottom_borders(t_str* map_buffer);
int
	check_left_right_borders(t_str* map_buffer);
int
	check_valid(t_config* config, t_str* map_buffer);

/* ************************************************************************** */
// 行内の '1' の数を数え、有効な文字のみが含まれているかチェックする
int
	count_check_columns(char const* line)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (line[i]) {
		if (line[i] == '1') {
			j++;
		} else if (line[i] != ' ') {
			return (0);
		}
		i++;
	}
	return (j);
}

/* ************************************************************************** */
// マップの上端と下端の境界が正しいかチェックする
int
	check_top_bottom_borders(t_str* map_buffer)
{
	int		first_line;
	int		last_line;
	t_str*	last;

	if (!map_buffer) {
		return (0);
	}
	first_line = count_check_columns(map_buffer->content);
	last = str_last(map_buffer);
	if (last) {
		last_line = count_check_columns(last->content);
	} else {
		last_line = 0;
	}
	if (first_line == last_line) {
		return (first_line);
	}
	return (0);
}

/* ************************************************************************** */
// マップの左端と右端の境界が正しいかチェックする
int
	check_left_right_borders(t_str* map_buffer)
{
	int	i;
	int	first;
	int	last;

	if (!map_buffer) {
		return (0);
	}
	i = 0;
	while (map_buffer) {
		first = 0;
		while (map_buffer->content[first] == ' ') {
			first++;
		}
		last = ft_strlen(map_buffer->content) - 1;
		while (last > 0 && map_buffer->content[last] == ' ') {
			last--;
		}
		printf("DEBUG row %d: first='%c'(%d) last='%c'(%d) len=%d [%s]\n",
			i, map_buffer->content[first], map_buffer->content[first],
			map_buffer->content[last], map_buffer->content[last],
			ft_strlen(map_buffer->content), map_buffer->content);
		if (last <= 1 || map_buffer->content[first] != '1' || map_buffer->content[last] != '1') {
			printf("DEBUG --> rejected at row %d\n", i);
			return (0);
		}
		map_buffer = map_buffer->next;
		i++;
	}
	printf("DEBUG check_LR returning i=%d\n", i);
	return (i);
}

/* ************************************************************************** */
// マップ全体が有効な文字のみで構成され、列数が正しいかチェックする
int
	check_valid(t_config* config, t_str* map_buffer)
{
	int	i;
	int	col;

	while (map_buffer) {
		i = 0;
		col = 0;
		while (map_buffer->content[i]) {
			if (!ft_in_set(map_buffer->content[i], VALID_MAP_CHARACTERS)) {
				return (0);
			}
			if (map_buffer->content[i++] != ' ') {
				col++;
			}
		}
		if (col != config->map.columns) {
			return (0);
		}
		map_buffer = map_buffer->next;
	}
	return (1);
}
