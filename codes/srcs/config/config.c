/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum  <samatsum@student.42.jp   >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 07:32:33 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/03 08:23:42 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config/config.h"
#include <stdio.h>

/* ************************************************************************** */
void
	init_config(t_config* config);
int
	clear_config(t_config* config);
int
	parse_config(t_config* config, char const* conf_path);
int
	parse_line(t_config* config, char const* line, t_str** map_buffer);
static int
	config_key(char const* line);

/* ************************************************************************** */
// 設定情報を初期化する
void
	init_config(t_config* config)
{
	int i;

	config->requested_width = 848;
	config->requested_height = 480;
	i = 0;
	while (i < TEXTURES) {
		config->tex_path[i++] = NULL;
	}
	config->colors[TEX_NORTH] = 0xFFFFFF;
	config->colors[TEX_SOUTH] = 0xCCCCCC;
	config->colors[TEX_WEST] = 0xFF44FF;
	config->colors[TEX_EAST] = 0x44FF44;
	config->colors[TEX_SKY] = 0x33C6E3;
	config->colors[TEX_FLOOR] = 0xA0764C;
	config->map.data = NULL;
	config->map.rows = 0;
	config->map.columns = 0;
	config->rotate_speed = .11;
	config->move_speed = .11;
	config->fov = .66;
	i = 0;
	while (i < C_LAST) {
		config->set[i++] = 0;
	}
}

/* ************************************************************************** */
// 設定情報に関するメモリを解放する
int
	clear_config(t_config* config)
{
	int i;

	i = 0;
	while (i < TEXTURES) {
		if (config->tex_path[i]) {
			free(config->tex_path[i]);
		}
		config->tex_path[i] = NULL;
		i++;
	}
	if (config->map.data) {
		free(config->map.data);
	}
	config->map.data = NULL;
	return (0);
}

/* ************************************************************************** */
// 設定ファイルを解析し、設定情報を読み込む
int
	parse_config(t_config* config, char const* conf_path)
{
	int		c_fd;
	int		r;
	char*	line;
	t_str*	map_buffer;

	if (!ft_endwith(conf_path, ".cub")) {
		printf("DEBUG: File extension is not .cub\n");
		return (0);
	}
	c_fd = open(conf_path, O_RDONLY);
	if (c_fd < 0) {
		printf("DEBUG: Failed to open file: %s\n", conf_path);
		return (0);
	}
	map_buffer = NULL;
	r = 1;
	while (get_next_line(c_fd, &line)) {
		r = (r && parse_line(config, line, &map_buffer));
		if (!r) {
			printf("DEBUG: parse_line failed at line: [%s]\n", line);
		}
		free(line);
	}
	if (r && ft_strlen(line) > 0) {
		r = !!str_add_back(&map_buffer, ft_strdup(line));
	}
	free(line);
	close(c_fd);
	if (!r) {
		printf("DEBUG: parse_line returned 0 during file reading.\n");
		return (str_clear(&map_buffer));
	}
	if (!parse_map(config, map_buffer)) {
		printf("DEBUG: parse_map failed (Invalid map structure).\n");
		return (str_clear(&map_buffer));
	}
	str_clear(&map_buffer);
	return (1);
}

/* ************************************************************************** */
// 読み込んだ行を解析し、適切な処理に振り分ける
int
	parse_line(t_config* config, char const* line, t_str** map_buffer)
{
	static int	empty_in_map = 0;
	static int	content_after = 0;
	int			length;
	int			key;

	length = ft_strlen(line);
	if (length == 0 && config->set[C_MAP]) {
		empty_in_map = 1;
	}
	if (empty_in_map && content_after) {
		return (0);
	}
	if (length == 0) {
		return (1);
	}
	key = config_key(line);
	if (key != C_MAP && (config->set[key] || config->set[C_MAP])) {
		return (0);
	}
	if (key == C_R) {
		return (parse_dimensions(config, line));
	} else if (key >= C_NO && key <= C_ST) {
		return (parse_texture(config, key, line));
	} else if (key == C_F || key == C_C) {
		return (parse_color(config, key, line));
	}
	config->set[key] = 1;
	if (empty_in_map) {
		content_after = 1;
	}
	return (!!str_add_back(map_buffer, ft_strdup(line)));
}

/* ************************************************************************** */
// 行の先頭文字列から設定キーを判定する
static int
	config_key(char const* line)
{
	if (line[0] == 'R' && line[1] == ' ') {
		return (C_R);
	} else if (line[0] == 'N' && line[1] == 'O') {
		return (C_NO);
	} else if (line[0] == 'S' && line[1] == 'O') {
		return (C_SO);
	} else if (line[0] == 'W' && line[1] == 'E') {
		return (C_WE);
	} else if (line[0] == 'E' && line[1] == 'A') {
		return (C_EA);
	} else if (line[0] == 'S' && line[1] == 'T') {
		return (C_ST);
	} else if (line[0] == 'F' && line[1] == 'T') {
		return (C_FT);
	} else if (line[0] == 'F' && line[1] == ' ') {
		return (C_F);
	} else if (line[0] == 'C' && line[1] == ' ') {
		return (C_C);
	} else if (line[0] == 'O' && line[1] == 'I') {
		return (C_OI);
	} else if (line[0] == 'O' && line[1] == 'P') {
		return (C_OP);
	} else if (line[0] == 'O' && line[1] == 'C') {
		return (C_OC);
	}
	return (C_MAP);
}
