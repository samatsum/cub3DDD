#include <sys/time.h>
#include <stdio.h>
#include "engine/render/render.h"
#include "engine/texture/texture.h"
#include "core/core.h"

/* ************************************************************************** */
void
	draw_weapon(t_game* game);
static void
	draw_overlay(t_game* game, t_tex* tex, double start_x, double start_y, double scale);

/* ************************************************************************** */
// テクスチャを画面上の指定位置にオーバレイ描画するヘルパー関数
static void
	draw_overlay(t_game* game, t_tex* tex, double start_x, double start_y, double scale)
{
	t_pos	pixel;
	t_pos	p_tex;
	int		color;

	if (!tex->tex) {
		return ;
	}
	pixel.y = start_y;
	while (pixel.y < game->window.size.y) {
		pixel.x = start_x;
		while (pixel.x < start_x + (tex->width * scale)) {
			p_tex.x = (int)((pixel.x - start_x) / scale);
			p_tex.y = (int)((pixel.y - start_y) / scale);

			if (p_tex.x >= 0 && p_tex.x < tex->width && p_tex.y >= 0 && p_tex.y < tex->height) {
				color = get_tex_color(tex, &p_tex);
				
				/* 透過判定（黒色をスキップ） */
				if ((color & 0x00FFFFFF) != 0x000000) {
					draw_pixel(&game->window, &pixel, color);
				}
			}
			pixel.x++;
		}
		pixel.y++;
	}
}

/* ************************************************************************** */
// 状態に合わせて武器や手のテクスチャを描画する
void
	draw_weapon(t_game* game)
{
	struct timeval	tv;
	long long		current_time;
	static long long last_update = 0;
	t_tex*			active_tex;
	double			scale;
	double			start_x;
	double			start_y;

	gettimeofday(&tv, NULL);
	current_time = (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000);

	/* アニメーションタイマーの更新 */
	if (game->is_shooting > 0 && (current_time - last_update) >= 100) {
		game->is_shooting--;
		last_update = current_time;
	}

	/* 両手の描画モード（左右に分けて描画） */
	if (game->current_weapon == WEP_HANDS) {
		/* 左手の描画 */
		active_tex = &game->weapon_tex[4];
		if (active_tex->tex) {
			scale = (game->window.size.y * 0.6) / active_tex->height;
			start_x = 0; /* 画面左端 */
			start_y = game->window.size.y - (active_tex->height * scale);
			draw_overlay(game, active_tex, start_x, start_y, scale);
		}
		/* 右手の描画 */
		active_tex = &game->weapon_tex[5];
		if (active_tex->tex) {
			scale = (game->window.size.y * 0.6) / active_tex->height;
			start_x = game->window.size.x - (active_tex->width * scale); /* 画面右端 */
			start_y = game->window.size.y - (active_tex->height * scale);
			draw_overlay(game, active_tex, start_x, start_y, scale);
		}
		return ;
	}

	/* ピストル または フラッシュライトの描画（中央配置） */
	if (game->current_weapon == WEP_PISTOL) {
		if (game->is_shooting > 7) {
			active_tex = &game->weapon_tex[1]; /* 発砲中 */
		} else if (game->is_shooting > 0) {
			active_tex = &game->weapon_tex[2]; /* 反動中 */
		} else {
			active_tex = &game->weapon_tex[0]; /* 待機中 */
		}
	} else {
		active_tex = &game->weapon_tex[3];     /* 懐中電灯 */
	}

	if (!active_tex->tex) {
		return ;
	}

	scale = (game->window.size.y * 0.6) / active_tex->height;
	start_x = (game->window.size.x / 2.0) - ((active_tex->width * scale) / 2.0);
	start_y = game->window.size.y - (active_tex->height * scale);
	
	draw_overlay(game, active_tex, start_x, start_y, scale);
}
