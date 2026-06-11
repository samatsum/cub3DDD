/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_params.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 15:25:21 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/11 18:13:18 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "config/config.h"

/* ************************************************************************** */
int
	parse_dimensions(t_config* config, char const* line);
int
	parse_color(t_config* config, int key, char const* line);
static int
	str_to_color(t_str* str);
int
	parse_speed(t_config* config, int key, char const* line);

/* ************************************************************************** */
// 画面サイズの解像度設定を解析する
int
	parse_dimensions(t_config* config, char const* line)
{
	int		i;
	int		tmp;
	t_str*	str;
	t_str*	param;

	i = 0;
	while (line[++i]) {
		if (line[i] != ' ' && line[i] < '0' && line[i] > '9') {
			return (0);
		}
	}
	str = NULL;
	str = ft_split(line, ' ');
	if (!str || str_length(str) != 3) {
		/* 修正2: ショートサーキット (||) や (|) による str_clear の実行漏れ・可読性低下を防ぐため、明示的に処理を分割 */
		str_clear(&str);
		return (0);
	}
	param = str->next;
	tmp = ft_atoi(param->content);
	if (tmp <= 1) {
		str_clear(&str);
		return (0);
	}
	config->requested_width = tmp;
	param = param->next;
	tmp = ft_atoi(param->content);
	if (tmp <= 1) {
		str_clear(&str);
		return (0);
	}
	config->requested_height = tmp;
	
	/* 成功時も確実にメモリを解放して 1 を返す */
	str_clear(&str);
	return (1);
}

/* ************************************************************************** */
// 色設定の文字列を解析し、設定に格納する
int
	parse_color(t_config* config, int key, char const* line)
{
	int				i;
	unsigned int	color;
	t_str*			str_arr[2];

	i = 1;
	while (line[i]) {
		if (!ft_in_set(line[i++], " ,0123456789")) {
			return (0);
		}
	}
	str_arr[0] = NULL;
	str_arr[1] = NULL;
	str_arr[0] = ft_split(line, ' ');
	if (!str_arr[0] || str_length(str_arr[0]) != 2) {
		str_clear(&str_arr[0]);
		str_clear(&str_arr[1]);
		return (0);
	}
	str_arr[1] = ft_split(str_arr[0]->next->content, ',');
	if (!str_arr[1] || str_length(str_arr[1]) != 3) {
		str_clear(&str_arr[0]);
		str_clear(&str_arr[1]);
		return (0);
	}
	color = (unsigned int)str_to_color(str_arr[1]);
	if ((int)color < 0) {
		str_clear(&str_arr[0]);
		str_clear(&str_arr[1]);
		return (0);
	}
	if (key == C_F) {
		config->colors[TEX_FLOOR] = color;
	} else {
		config->colors[TEX_SKY] = color;
	}
	
	str_clear(&str_arr[0]);
	str_clear(&str_arr[1]);
	return (1);
}

/* ************************************************************************** */
// 文字列リストから色情報を数値として計算する
static int
	str_to_color(t_str* str)
{
	int	i;
	int	color;
	int	tmp;

	i = 0;
	color = 0;
	while (str) {
		tmp = ft_atoi(str->content);
		if (tmp < 0 || tmp > 255) {
			return (-1);
		}
		color = color | (tmp << (16 - (i++ * 8)));
		str = str->next;
	}
	return (color);
}

int
	parse_speed(t_config* config, int key, char const* line)
{
	int		i;
	double	speed;
	char*	endptr;

	i = 1;//MS,RSは２文字なので。
	while (line[++i]) {
		// 	printf("line[%d] = '%c'\n", i, line[i]); // デバッグ用出力
		if (!ft_in_set(line[i], " .0123456789")) {
			return (0);
		}
	}
	// printf("line to parse: '%s'\n", line); // デバッグ用出力
	// printf("line + 2 to parse: '%s'\n", line + 2); // デバッグ用出力
	speed = strtod(line + 2, &endptr);
	// printf("Parsed speed: %f\n", speed); // デバッグ用出力
	if (endptr == line + 2 || speed <= 0.0) {
		return (0);
	}
	if (key == C_RS) {
		config->rotate_speed = speed;
	} else if (key == C_MS) {
		config->move_speed = speed;
	}
	return (1);
}