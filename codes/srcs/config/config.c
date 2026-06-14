#include <stdio.h>
#include <fcntl.h>  /* open, O_RDONLY 用 */
#include <unistd.h> /* close 用 */
#include "config/config.h"
#include "gnl/get_next_line.h" /* get_next_line 用 */
#include "config/defaults.h"

/* ************************************************************************** */
// 行頭キー文字列と種別の対応表。新しいキーはここに1行追加するだけでよい
typedef struct s_key_def
{
	char const*	tag;
	int			key;
}				t_key_def;

/* ************************************************************************** */
// 行頭キー文字列と種別の対応表。.cub で記述できる設定キーの一覧でもある。
// オブジェクトは各カテゴリ最大5種(OI1..OI5 等)。OI/OP/OC は旧1種目の別名
static const t_key_def	g_keys[] = {
	{"R", C_R},      // 解像度          : R 幅 高さ
	{"NO", C_NO},    // 壁テクスチャ    : 北向き (.xpm)
	{"SO", C_SO},    // 壁テクスチャ    : 南向き (.xpm)
	{"WE", C_WE},    // 壁テクスチャ    : 西向き (.xpm)
	{"EA", C_EA},    // 壁テクスチャ    : 東向き (.xpm)
	{"ST", C_ST},    // 天井テクスチャ  : 任意 (.xpm)
	{"FT", C_FT},    // 床テクスチャ    : 任意 (.xpm)
	{"F", C_F},      // 床の色          : F R,G,B（テクスチャ未指定時のフォールバック）
	{"C", C_C},      // 天井の色        : C R,G,B（テクスチャ未指定時のフォールバック）
	{"OI1", C_OI1},  // 通行不可 1種目  : 障害物テクスチャ (.xpm)
	{"OI2", C_OI2},  // 通行不可 2種目
	{"OI3", C_OI3},  // 通行不可 3種目
	{"OI4", C_OI4},  // 通行不可 4種目
	{"OI5", C_OI5},  // 通行不可 5種目
	{"OP1", C_OP1},  // 通行可   1種目  : 装飾テクスチャ (.xpm)
	{"OP2", C_OP2},  // 通行可   2種目
	{"OP3", C_OP3},  // 通行可   3種目
	{"OP4", C_OP4},  // 通行可   4種目
	{"OP5", C_OP5},  // 通行可   5種目
	{"OC1", C_OC1},  // 収集     1種目  : アイテムテクスチャ (.xpm)
	{"OC2", C_OC2},  // 収集     2種目
	{"OC3", C_OC3},  // 収集     3種目
	{"OC4", C_OC4},  // 収集     4種目
	{"OC5", C_OC5},  // 収集     5種目
	{"MS", C_MS},    // 移動速度        : 任意 (0 より大)
	{"RS", C_RS},    // 回転速度        : 任意 (0 より大)
	{"FOV", C_FOV},  // 視野角          : 任意 (大きいほど広角)
	{"ET", C_ET},    // 敵の追跡秒数    : 任意 (見失うまでの秒数)
};

/* ************************************************************************** */
void
	init_config(t_config* config);
int
	clear_config(t_config* config);
int
	parse_config(t_config* config, char const* conf_path);
static int
	parse_line(t_config* config, char const* line, t_str** map_buffer, int* empty_map, int* cont_after);
static int
	config_key(char const* line);
static int
	tag_matches(char const* line, char const* tag);

/* ************************************************************************** */
// 設定情報を初期化する
void
	init_config(t_config* config)
{
	int	i;

	config->requested_width = MIN_WIDTH;
	config->requested_height = MIN_HEIGHT;
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
	config->map.flags = NULL;
	config->map.rows = 0;
	config->map.columns = 0;
	config->rotate_speed = DEFAULT_ROTATE_SPEED;
	config->move_speed = DEFAULT_MOVE_SPEED;
	config->fov = DEFAULT_FOV;
	config->enemy_track_seconds = DEFAULT_ENEMY_TRACK_SECONDS;
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
	int	i;

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
	if (config->map.flags) {
		free(config->map.flags);
	}
	config->map.flags = NULL;
	return (0);
}

/* ************************************************************************** */
// 設定ファイルを解析し、設定情報を読み込む
int
	parse_config(t_config* config, char const* conf_path)
{
	int		c_fd;
	int		ret;
	int		r;
	int		empty_map;
	int		cont_after;
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
	empty_map = 0;
	cont_after = 0;
	line = NULL;
	ret = get_next_line(c_fd, &line);
	while (ret > 0) {
		r = (r && parse_line(config, line, &map_buffer, &empty_map, &cont_after));
		if (!r) {
			printf("DEBUG: parse_line failed at line: [%s]\n", line);
		}
		free(line);
		line = NULL;
		ret = get_next_line(c_fd, &line);
	}
	close(c_fd);
	if (ret < 0) {
		r = 0;
	}
	if (!r) {
		printf("DEBUG: parse failed during file reading.\n");
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
static int
	parse_line(t_config* config, char const* line, t_str** map_buffer, int* empty_map, int* cont_after)
{
	int	length;
	int	key;

	length = ft_strlen(line);
	if (length == 0 && config->set[C_MAP]) {
		*empty_map = 1;
	}
	if (*empty_map && *cont_after) {
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
	} else if (key >= C_MS && key <= C_ET) {
		return (parse_scalar(config, key, line));
	}
	config->set[key] = 1;
	if (*empty_map) {
		*cont_after = 1;
	}
	return (!!str_add_back(map_buffer, ft_strdup(line)));
}

/* ************************************************************************** */
// 行の先頭トークンを対応表と照合し、設定キーを判定する
static int
	config_key(char const* line)
{
	size_t	i;

	i = 0;
	while (i < sizeof(g_keys) / sizeof(g_keys[0])) {
		if (tag_matches(line, g_keys[i].tag)) {
			return (g_keys[i].key);
		}
		i++;
	}
	return (C_MAP);
}

/* ************************************************************************** */
// 行頭が tag と一致し、かつ直後がトークン境界（空白か行末）であるかを判定する
static int
	tag_matches(char const* line, char const* tag)
{
	int	i;

	i = 0;
	while (tag[i]) {
		if (line[i] != tag[i]) {
			return (0);
		}
		i++;
	}
	return (line[i] == ' ' || line[i] == '\0');
}
