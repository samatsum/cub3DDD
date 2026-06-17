#ifndef TYPES_H
# define TYPES_H

# include <sys/time.h>
# include "config/config.h"
# include "engine/raycast/raycast.h"
# include "engine/render/render.h"
# include "enemy/enemy_types.h"

/* ************************************************************************** */
// 描画オプションの状態フラグ（UI・影・照準・スクリーンショット）
# define FLAG_UI			0x00000010
# define FLAG_SHADOWS		0x00000100
# define FLAG_CROSSHAIR		0x00001000
# define FLAG_SAVE			0x00000001
# define FLAG_FLASHLIGHT	0x00010000

/* ************************************************************************** */
// 不完全構造体の前方宣言
typedef struct s_sprite		t_sprite;
typedef struct s_enemy		t_enemy;
typedef struct s_tex		t_tex;
typedef struct s_light		t_light;

// 装備中の武器の種別
typedef enum e_weapon_type
{
	WEP_PISTOL = 0,
	WEP_FLASHLIGHT,
	WEP_HANDS
}				t_weapon_type;

// ゲームモード（FPS=通常モード / RSP=じゃんけん鬼ごっこモード）
typedef enum e_game_mode
{
	MODE_FPS = 0,
	MODE_RSP
}				t_game_mode;

// 武器・手のテクスチャ配列のインデックス
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
	t_pos			move;
	t_pos			x_move;
	t_pos			rotate;
	int				current_weapon;
	int				is_shooting;
}				t_input;

// ゲーム世界の動的エンティティと収集進行状況
typedef struct s_world
{
	t_sprite*		sprites;
	t_enemy*		enemies;
	t_light*		lights;
	int				light_count;
	int				to_collect;
	int				collected;
}				t_world;

// 画像アセット（壁/床/天井・武器・敵・扉・死亡画面のテクスチャ群）。
// hand_tex は RSPモード専用で team * HAND_COUNT + hand の並びで引く6枚
// （Hand_<Team>_<Hand>.xpm）。FPSモードでは読み込まず未使用
typedef struct s_assets
{
	t_tex			tex[TEXTURES];
	t_tex			weapon_tex[WEAPON_TEX_COUNT];
	t_tex			enemy_tex[ENEMY_TEX_COUNT];
	t_tex			hand_tex[TEAM_COUNT * HAND_COUNT];
	t_tex			door_tex;
	t_tex			death_tex;
}				t_assets;

// 描画前計算のキャッシュ（カメラ平面比率・深度・床天井距離・回転三角関数）
typedef struct s_render_cache
{
	double			camera_x[MAX_WIDTH];
	double			depth[MAX_WIDTH];
	double			sf_dist[MAX_HEIGHT];
}				t_render_cache;

// フレーム制御用のタイミング情報
typedef struct s_timing
{
	long long		last_time;
}				t_timing;

// 各サブシステムを集約するファサード構造体（spawn は初期スポーン状態のスナップショット、
// death_timer は死亡演出の残り秒数で 0 超なら死亡中＝全画面の死亡画像を表示する。
// player_rsp/rsp_seed は RSPモード専用。player_rsp はプレイヤーの team/hand/spawn/alive、
// rsp_seed は rsp_rehand 用の乱数状態。FPSモードでは未使用。
// mode は e_game_mode の値（MODE_FPS / MODE_RSP）で、argv[2] の有無で決まる。
// rsp_on_home は前フレームでプレイヤーが自陣スポーンマスに乗っていたかで、
// 自陣を踏み直した瞬間に手を変える検出に使う）
typedef struct s_game
{
	t_config		config;
	t_window		window;
	t_camera		camera;
	t_camera		spawn;
	t_input			input;
	t_world			world;
	t_assets		assets;
	t_render_cache	cache;
	t_timing		timing;
	t_rsp_state		player_rsp;
	unsigned int	rsp_seed;
	double			death_timer;
	int				options;
	int				last_options;
	int				mode;
	int				rsp_on_home;
}				t_game;

#endif
