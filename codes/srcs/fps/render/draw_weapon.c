#include <math.h>
#include <sys/time.h>
#include "core/core.h"
#include "engine/render/render.h"
#include "engine/texture/texture.h"

/* ************************************************************************** */
#define SHOOT_INTERVAL		100
#define SHOOT_FRAMES_MAX	7
#define BOB_PERIOD			2000.0
#define BOB_POWER			3.0
#define WEAPON_SCALE		0.6
// 銃/ライト(FPS)とRSPの手の表示サイズ。3キーの走行用の手(WEAPON_SCALE)より
// 小さく、その約2/3に縮小する。走行用の手はこの縮小の対象外
#define WEAPON_SCALE_SMALL	0.4
#define HAND_MOVE_RATIO		0.8

void
	draw_weapon(t_game* game);
static void
	render_rsp_hand(t_game* game);
static void
	update_weapon_timer(t_game* game, long long current_time);
static void
	render_hands(t_game* game, long long current_time);
static void
	render_item(t_game* game);
static void
	draw_overlay(t_game* game, t_tex* tex, double start_x, double start_y, double scale);

/* ************************************************************************** */
// 状態に合わせて武器や手のテクスチャを描画する（司令塔）。RSPでは銃を出さず、
// 自分のじゃんけんの手を画面下部に常時表示する
void
	draw_weapon(t_game* game)
{
	struct timeval	tv;
	long long		current_time;

	if (game->mode == MODE_RSP) {
		render_rsp_hand(game);
		return ;
	}
	gettimeofday(&tv, NULL);
	current_time = (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000);
	update_weapon_timer(game, current_time);
	if (game->input.current_weapon == WEP_HANDS) {
		render_hands(game, current_time);
	} else {
		render_item(game);
	}
}

/* ************************************************************************** */

// RSPのプレイヤーUI。自分のチーム×手のハンドテクスチャを画面下部中央へ描画する
static void
	render_rsp_hand(t_game* game)
{
	t_tex*	tex;
	double	scale;
	double	start_x;
	double	start_y;

	tex = &game->assets.hand_tex[game->player_rsp.team * HAND_COUNT + game->player_rsp.hand];
	if (!tex->tex) {
		return ;
	}
	scale = (game->window.size.y * WEAPON_SCALE_SMALL) / tex->height;
	start_x = (game->window.size.x - (tex->width * scale)) / 2.0;
	start_y = game->window.size.y - (tex->height * scale);
	draw_overlay(game, tex, start_x, start_y, scale);
}

/* ************************************************************************** */
// 射撃アニメーションのタイマー（状態）を更新する
static void
	update_weapon_timer(t_game* game, long long current_time)
{
	static long long	last_update = 0;

	if (game->input.is_shooting > 0 && (current_time - last_update) >= SHOOT_INTERVAL) {
		game->input.is_shooting--;
		last_update = current_time;
	}
}

/* ************************************************************************** */
// 移動時の揺れ（ボビング）を計算し、両手を描画する
static void
	render_hands(t_game* game, long long current_time)
{
	t_tex*	active_tex;
	double	scale;
	double	start_x;
	double	start_y;
	double	angle;
	double	move_dist;
	double	bob_left;
	double	bob_right;
	int		is_moving;

	is_moving = game->input.move.x || game->input.move.y || game->input.x_move.x || game->input.x_move.y;
	if (!is_moving) {
		return ;
	}
	angle = (double)(current_time % (long long)BOB_PERIOD) / BOB_PERIOD * 2.0 * M_PI;
	bob_left = pow(fabs(sin(angle)), BOB_POWER);
	bob_right = pow(fabs(cos(angle)), BOB_POWER);
	
	active_tex = &game->assets.weapon_tex[WTEX_HAND_LEFT];
	if (active_tex->tex) {
		scale = (game->window.size.y * WEAPON_SCALE) / active_tex->height;
		move_dist = active_tex->height * scale * HAND_MOVE_RATIO;
		start_x = 0 - move_dist * bob_left;
		start_y = game->window.size.y - (active_tex->height * scale) + move_dist * bob_left;
		draw_overlay(game, active_tex, start_x, start_y, scale);
	}
	active_tex = &game->assets.weapon_tex[WTEX_HAND_RIGHT];
	if (active_tex->tex) {
		scale = (game->window.size.y * WEAPON_SCALE) / active_tex->height;
		move_dist = active_tex->height * scale * HAND_MOVE_RATIO;
		start_x = game->window.size.x - (active_tex->width * scale) + move_dist * bob_right;
		start_y = game->window.size.y - (active_tex->height * scale) + move_dist * bob_right;
		draw_overlay(game, active_tex, start_x, start_y, scale);
	}
}

/* ************************************************************************** */
// ピストルまたはフラッシュライトの適切なアニメーションフレームを描画する
static void
	render_item(t_game* game)
{
	t_tex*	active_tex;
	double	scale;
	double	start_x;
	double	start_y;

	if (game->input.current_weapon == WEP_PISTOL) {
		if (game->input.is_shooting > SHOOT_FRAMES_MAX) {
			active_tex = &game->assets.weapon_tex[WTEX_PISTOL_SHOOT];
		} else if (game->input.is_shooting > 0) {
			active_tex = &game->assets.weapon_tex[WTEX_PISTOL_RECOIL];
		} else {
			active_tex = &game->assets.weapon_tex[WTEX_PISTOL_IDLE];
		}
	} else if (game->input.current_weapon == WEP_FLASHLIGHT) {
		active_tex = &game->assets.weapon_tex[WTEX_FLASHLIGHT];
	} else {
		return ;
	}
	if (!active_tex->tex) {
		return ;
	}
	scale = (game->window.size.y * WEAPON_SCALE_SMALL) / active_tex->height;
	start_x = (game->window.size.x / 2.0) - ((active_tex->width * scale) / 2.0);
	start_y = game->window.size.y - (active_tex->height * scale);
	draw_overlay(game, active_tex, start_x, start_y, scale);
}

/* ************************************************************************** */
// テクスチャを画面上の指定位置・スケールで描画する（ループの無駄を排除した最適化版）
static void
	draw_overlay(t_game* game, t_tex* tex, double start_x, double start_y, double scale)
{
	t_pos	pixel;
	t_pos	p_tex;
	int		color;
	double	inv_scale;

	if (!tex->tex || scale == 0.0) {
		return ;
	}
	inv_scale = 1.0 / scale;
	
	pixel.y = start_y < 0 ? 0 : start_y;
	while (pixel.y < game->window.size.y && pixel.y < start_y + (tex->height * scale)) {
		p_tex.y = (int)((pixel.y - start_y) * inv_scale);
		if (p_tex.y >= 0 && p_tex.y < tex->height) {
			pixel.x = start_x < 0 ? 0 : start_x;
			while (pixel.x < game->window.size.x && pixel.x < start_x + (tex->width * scale)) {
				p_tex.x = (int)((pixel.x - start_x) * inv_scale);
				if (p_tex.x >= 0 && p_tex.x < tex->width) {
					color = get_tex_color(tex, &p_tex);
					if ((color & 0x00FFFFFF) != 0x000000) {
						draw_pixel(&game->window, &pixel, color);
					}
				}
				pixel.x++;
			}
		}
		pixel.y++;
	}
}
