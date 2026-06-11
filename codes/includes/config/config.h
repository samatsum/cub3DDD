#ifndef CONFIG_H
# define CONFIG_H

/* ************************************************************************** */
# include <math.h> /* floor関数を使用するマクロ(FINT)のため */
# include "utils/utils.h"

/* ************************************************************************** */
// マップ解析で使用する方向文字と有効なマップ文字の集合
# define DIRECTIONS				"NSEW"
# define VALID_MAP_CHARACTERS	" 01234EWNSM"

// ウィンドウ解像度の上限・下限
# define MAX_WIDTH				1920
# define MAX_HEIGHT				1080
# define MIN_WIDTH				848
# define MIN_HEIGHT				480

// 視野角(FOV)の調整スケールと最適アスペクト比
# define FOV_SCALE				2.5
# define BEST_RATIO				1.7777777778

// 座標の整数化・マップ範囲判定・マップ要素参照を行うマクロ群
# define FINT(x)				((int)floor(x))
# define CHECK_TOP(p)			(FINT(p.x) >= 0 && FINT(p.y) >= 0)
# define CHECK_BOT(p, c)		(FINT(p.x) < (c).map.columns && FINT(p.y) < (c).map.rows)
# define IN_MAP(p, c)			(CHECK_TOP(p) && CHECK_BOT(p, c))
# define MAP(p, c)				(c).map.data[(FINT(p.y) * (c).map.columns) + FINT(p.x)]
# define MAP_XY(x, y, c)		(c).map.data[(FINT(y) * (c).map.columns) + FINT(x)]

/* ************************************************************************** */
// 設定ファイルの行種別キー（順序・値はパーサの範囲判定に依存するため変更不可。
// C_MAP はマップ本体、C_LAST は set[] の要素数を兼ねるため必ず末尾に置く）
typedef enum e_config_key
{
	C_R = 0,
	C_NO,
	C_SO,
	C_WE,
	C_EA,
	C_OI,
	C_OP,
	C_OC,
	C_FT,
	C_ST,
	C_F,
	C_C,
	C_MS,
	C_RS,
	C_MAP,
	C_LAST
}				t_config_key;

// テクスチャ／色配列のインデックス（TEXTURES は要素数を兼ねるため必ず末尾に置く）
typedef enum e_texture_id
{
	TEX_NORTH = 0,
	TEX_SOUTH,
	TEX_WEST,
	TEX_EAST,
	TEX_SKY,
	TEX_FLOOR,
	TEX_SPRITE,
	TEX_SPRITE_UP,
	TEX_SPRITE_C,
	TEXTURES
}				t_texture_id;

// マップ本体（1次元配列）と寸法を保持する構造体
typedef struct s_map
{
	int*	data;
	int		columns;
	int		rows;
}				t_map;

// 解像度・色・テクスチャパス・速度・マップなど全設定を集約する構造体
typedef struct s_config
{
	char*			tex_path[TEXTURES];
	double			rotate_speed;
	double			move_speed;
	double			fov;
	unsigned int	requested_width;
	unsigned int	requested_height;
	unsigned int	colors[TEXTURES];
	int				set[C_LAST];
	t_map			map;
}				t_config;

/* ************************************************************************** */
void
	init_config(t_config* config);
int
	clear_config(t_config* config);
int
	parse_dimensions(t_config* config, char const* line);
int
	parse_texture(t_config* config, int key, char const* line);
int
	parse_color(t_config* config, int key, char const* line);
int
	parse_speed(t_config* config, int key, char const* line);
int
	parse_config(t_config* config, char const* conf_path);
int
	count_check_columns(char const* line);
int
	check_top_bottom_borders(t_str* map_buffer);
int
	check_left_right_borders(t_str* map_buffer);
int
	check_valid(t_config* config, t_str* map_buffer);
int
	parse_map(t_config* config, t_str* map_buffer);

#endif
