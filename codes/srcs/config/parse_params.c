/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_params.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 15:25:21 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/03 08:35:20 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config/config.h"

/* ************************************************************************** */
int
	parse_dimensions(t_config* config, char const* line);
int
	parse_color(t_config* config, int key, char const* line);
static int
	str_to_color(t_str* str);

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
		return (str_clear(&str));
	}
	param = str->next;
	tmp = ft_atoi(param->content);
	if (tmp <= 1) {
		return (str_clear(&str));
	}
	config->requested_width = tmp;
	param = param->next;
	tmp = ft_atoi(param->content);
	if (tmp <= 1) {
		return (str_clear(&str));
	}
	config->requested_height = tmp;
	return (str_clear(&str) | 1);
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
		return (str_clear(&str_arr[0]) || str_clear(&str_arr[1]));
	}
	str_arr[1] = ft_split(str_arr[0]->next->content, ',');
	if (!str_arr[1] || str_length(str_arr[1]) != 3) {
		return (str_clear(&str_arr[0]) || str_clear(&str_arr[1]));
	}
	color = (unsigned int)str_to_color(str_arr[1]);
	if ((int)color < 0) {
		return (str_clear(&str_arr[0]) || str_clear(&str_arr[1]));
	}
	if (key == C_F) {
		config->colors[TEX_FLOOR] = color;
	} else {
		config->colors[TEX_SKY] = color;
	}
	return ((str_clear(&str_arr[0]) || str_clear(&str_arr[1])) | 1);
}

/* ************************************************************************** */
// 文字列リストから色情報を数値として計算する
static int
	str_to_color(t_str* str)
{
	int i;
	int color;
	int tmp;

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
