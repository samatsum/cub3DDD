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
// 新しいキーはここに1行追加するだけでよい
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
	{"OI", C_OI},    // スプライト '2'  : 通行不可の障害物テクスチャ (.xpm)
	{"OP", C_OP},    // スプライト '3'  : 通行可能な装飾テクスチャ (.xpm)
	{"OC", C_OC},    // スプライト '4'  : 収集アイテムのテクスチャ (.xpm)
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
	/* パース状態をローカル変数として管理し、再実行時の状態残留バグを回避する */
	empty_map = 0;
	cont_after = 0;
	/* get_next_line は「成功:1 / 終端:0 / エラー:負, 終端・エラー時 *line=NULL」を保証するため、
	   line を NULL 始点とし ret>0 の間だけ処理する。末尾改行なしの最終行も ret==1 で届くため、
	   ループ後に line を再参照する旧来の特殊処理（解放後参照・二重解放の温床）は不要になった */
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
	/* static変数を廃止し、引数で状態ポインタを受け取る（状態依存バグの回避） */
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