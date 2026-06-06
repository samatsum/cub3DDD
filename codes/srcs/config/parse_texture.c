/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_texture.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 13:33:50 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/07 06:39:56 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config/config.h"

/* ************************************************************************** */
int
	parse_texture(t_config* config, int key, char const* line);
static int
	texture_index(int key);
static char*
	path_from_line(int start, char const* line);

/* ************************************************************************** */
// テクスチャ設定を解析し、パスを保存する
int
	parse_texture(t_config* config, int key, char const* line)
{
	char*	path;
	int		index;

	index = texture_index(key);
	if (config->tex_path[index]) {
		free(config->tex_path[index]);
		config->tex_path[index] = NULL;
	}
	// 常に「先頭から2文字飛ばした先」からパスを取得
	path = path_from_line(2, line);
	if (!path) {
		return (0);
	}
	config->tex_path[index] = path;
	return (1);
}

/* ************************************************************************** */
// 設定キーからテクスチャのインデックスを取得する
static int
	texture_index(int key)
{
	if (key == C_NO) {
		return (TEX_NORTH);
	} else if (key == C_SO) {
		return (TEX_SOUTH);
	} else if (key == C_WE) {
		return (TEX_WEST);
	} else if (key == C_EA) {
		return (TEX_EAST);
	} else if (key == C_ST) {
		return (TEX_SKY);
	} else if (key == C_FT) {
		return (TEX_FLOOR);
	} else if (key == C_OI) {
		return (TEX_SPRITE);
	} else if (key == C_OP) {
		return (TEX_SPRITE_UP);
	} else if (key == C_OC) {
		return (TEX_SPRITE_C);
	}
	return (TEX_SPRITE);
}

/* ************************************************************************** */
// 行文字列からテクスチャのパス部分を抽出する
static char*
	path_from_line(int start, char const* line)
{
	int		start_def;
	int		end;
	char*	path;

	start_def = start;
	if (!line) {
		return (NULL);
	}
	while (line[start] && line[start] == ' ') {
		start++;
	}
	end = ft_strlen(line);
	while (line[end] == ' ') {
		end--;
	}
	if (start == start_def || end - start <= 0) {
		return (NULL);
	}
	path = ft_substr(line, start, end - start);
	if (!path) {
		return (NULL);
	}
	return (path);
}
