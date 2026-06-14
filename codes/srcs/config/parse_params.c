#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "config/config.h"
#include "tuning.h"

/* ************************************************************************** */
// 上書き可能なスカラー設定のレジストリ（キー→t_configフィールド→許容範囲）。
// 新しいスカラー設定を増やすときは、この配列に1行追加し、対応キーを enum と
// config_key に足すだけでよい（パーサ本体の分岐を増やす必要はない）。
// enemy_hp は double で受け、生成時に int へ縮小する（範囲検証は下限 0 超のみ）
typedef struct s_scalar_def
{
	int		key;
	double	min_exclusive;
	double	max_inclusive;
	size_t	field_off;
}				t_scalar_def;

static const t_scalar_def	g_scalars[] = {
	{C_MS, 0.0, 100.0, offsetof(t_config, move_speed)},
	{C_RS, 0.0, 100.0, offsetof(t_config, rotate_speed)},
	{C_FOV, 0.0, 10.0, offsetof(t_config, fov)},
	{C_ET, 0.0, 3600.0, offsetof(t_config, enemy_track_seconds)},
	{C_ES, 0.0, 100.0, offsetof(t_config, enemy_speed)},
	{C_EH, 0.0, 100000.0, offsetof(t_config, enemy_hp)},
};

/* ************************************************************************** */
int
	parse_dimensions(t_config* config, char const* line);
int
	parse_color(t_config* config, int key, char const* line);
int
	parse_scalar(t_config* config, int key, char const* line);
static int
	str_to_color(t_str* str);
static const t_scalar_def*
	find_scalar(int key);

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
// .cub のスカラー設定(MS/RS/FOV/ET/ES/EH)を解析し、対応フィールドを上書きする
int
	parse_scalar(t_config* config, int key, char const* line)
{
	const t_scalar_def*	def;
	double				value;
	char*				endptr;
	int					start;
	int					i;

	def = find_scalar(key);
	if (!def) {
		return (0);
	}
	start = 0;
	while ((line[start] >= 'A' && line[start] <= 'Z')
		|| (line[start] >= 'a' && line[start] <= 'z')) {
		start++;
	}
	i = start;
	while (line[i]) {
		if (!ft_in_set(line[i], " .0123456789")) {
			return (0);
		}
		i++;
	}
	value = strtod(line + start, &endptr);
	if (endptr == line + start || value <= def->min_exclusive
		|| value > def->max_inclusive) {
		return (0);
	}
	*(double*)((char*)config + def->field_off) = value;
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
		if (tmp < 0 || tmp > RGB_MAX) {
			return (-1);
		}
		color = color | (tmp << (16 - (i++ * 8)));
		str = str->next;
	}
	return (color);
}

/* ************************************************************************** */
// キーに対応するスカラー設定の定義をレジストリから検索する
static const t_scalar_def*
	find_scalar(int key)
{
	size_t	i;

	i = 0;
	while (i < sizeof(g_scalars) / sizeof(g_scalars[0])) {
		if (g_scalars[i].key == key) {
			return (&g_scalars[i]);
		}
		i++;
	}
	return (NULL);
}
