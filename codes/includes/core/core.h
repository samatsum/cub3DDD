#ifndef CORE_H
# define CORE_H

/* ************************************************************************** */
# include <sys/time.h> /* gettimeofday 等による時間計測のため */
# include "config/config.h"
# include "engine/render/render.h"
# include "engine/raycast/raycast.h"
# include "enemy/enemy.h"

/* ************************************************************************** */
// 描画オプションの状態フラグ（UI・影・照準・スクリーンショット）
# define FLAG_UI			0x00000010
# define FLAG_SHADOWS		0x00000100
# define FLAG_CROSSHAIR		0x00001000
# define FLAG_SAVE			0x00000001

/* ************************************************************************** */
// 装備中の武器の種別
typedef enum e_weapon_type
{
	WEP_PISTOL = 0,
	WEP_FLASHLIGHT,
	WEP_HANDS
}				t_weapon_type;

// 武器・手のテクスチャ配列のインデックス（WEAPON_TEX_COUNT は要素数を兼ねるため必ず末尾に置く）
typedef enum e_weapon_tex_id
{
	WTEX_PISTOL_IDLE = 0,
	WTEX_PISTOL_SHOOT,
	WTEX_PISTOL_RECOIL,
	WTEX_FLASHLIGHT,
	WTEX_HAND_LEFT,
	WTEX_HAND_RIGHT,
	WEAPON_TEX_COUNT
}				t_weapon_tex_id;

// 入力状態（押下中キーに応じた移動・回転フラグと武器の状態）
typedef struct s_input
{
	t_pos	move;
	t_pos	x_move;
	t_pos	rotate;
	int		current_weapon;
	int		is_shooting;
}				t_input;

// ゲーム世界の動的エンティティと収集進行状況
typedef struct s_world
{
	t_sprite*	sprites;
	t_enemy*	enemies;
	int			to_collect;
	int			collected;
}				t_world;

// 画像アセット（壁/床/天井・武器・敵のテクスチャ群）
typedef struct s_assets
{
	t_tex	tex[TEXTURES];
	t_tex	weapon_tex[WEAPON_TEX_COUNT];
	t_tex	enemy_tex[ENEMY_TEX_COUNT];
}				t_assets;

// 描画前計算のキャッシュ（カメラ平面比率・深度・床天井距離・回転三角関数）
typedef struct s_render_cache
{
	double	camera_x[MAX_WIDTH];
	double	depth[MAX_WIDTH];
	double	sf_dist[MAX_HEIGHT];
}				t_render_cache;

// フレーム制御用のタイミング情報
typedef struct s_timing
{
	long long	last_time;
}				t_timing;

// 各サブシステムを集約するファサード構造体
typedef struct s_game
{
	t_config		config;
	t_window		window;
	t_camera		camera;
	t_input			input;
	t_world			world;
	t_assets		assets;
	t_render_cache	cache;
	t_timing		timing;
	int				options;
	int				last_options;
}				t_game;

/* ************************************************************************** */
int
	main_loop(t_game* game);
void
	init_game(t_game* game);
int
	finish_init(t_game* game);
int
	exit_game(t_game* game, int code);
int
	exit_error(t_game* game, const char* str);
int
	screenshot(t_game* game);
void
	check_quest(t_game* game);
void
	count_items(t_game* game);
void
	shoot_target(t_game* game);

#endif
