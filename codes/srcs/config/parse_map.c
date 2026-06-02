/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 15:24:13 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/03 08:25:37 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config/config.h"

/* ************************************************************************** */
int
	parse_map(t_config* config, t_str* map_buffer);
int
	copy_map(t_config* config, t_str* map_buffer, int* map);

/* ************************************************************************** */
// バッファからマップデータを解析し、設定に格納する
int
	parse_map(t_config* config, t_str* map_buffer)
{
	int* map;

	map = NULL;
	config->map.columns = check_top_bottom_borders(map_buffer);
	config->map.rows = check_left_right_borders(map_buffer);
	if (config->map.columns <= 2 || config->map.rows <= 2 || !check_valid(config, map_buffer)) {
		return (0);
	}
	map = (int*)malloc(sizeof(*map) * (config->map.rows * config->map.columns));
	if (!map) {
		return (0);
	}
	if (copy_map(config, map_buffer, map) != 1) {
		return (0);
	}
	config->map.data = map;
	return (1);
}

/* ************************************************************************** */
// マップバッファの内容を配列にコピーし、カメラの数を返す
int
	copy_map(t_config* config, t_str* map_buffer, int* map)
{
	int i;
	int j;
	int line;
	int has_camera;

	i = 0;
	has_camera = 0;
	while (map_buffer) {
		j = 0;
		line = 0;
		while (map_buffer->content[j]) {
			while (map_buffer->content[j] == ' ') {
				j++;
			}
			map[(i * config->map.columns) + line++] = map_buffer->content[j];
			if (ft_in_set(map_buffer->content[j], DIRECTIONS)) {
				has_camera++;
			}
			j++;
		}
		map_buffer = map_buffer->next;
		i++;
	}
	return (has_camera);
}
