/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 13:16:59 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/03 15:13:43 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "core/core.h"            /* ゲーム設定の取得に必要 */
#include "engine/render/render.h"   /* スプライト描画関数の宣言 */
#include "engine/texture/texture.h" /* get_tex_color, distance_shade 等に必要 */

/* ************************************************************************** */
void
	draw_sprites(t_render* rnd, t_sprite* sprites);
static void
	init_draw_sprite(t_render* rnd, t_sprite* sprite, double inv_det, t_sprite_draw* spr);
static void
	draw_sprite(t_render* rnd, t_sprite* sprite, t_sprite_draw* spr, t_tex* tex);
static void
	draw_sprite_pixel(t_render* rnd, t_sprite* sprite, t_sprite_draw* spr, t_tex* tex);
static int
	set_tex_pos(t_render* rnd, t_sprite_draw* spr, t_tex* tex, t_pos* tex_pos);

/* ************************************************************************** */
// 全てのスプライトを距離順にソートして描画する
void
	draw_sprites(t_render* rnd, t_sprite* sprites)
{
	t_sprite*		sorted;
	t_sprite_draw	spr;
	double			inv_det;

	inv_det = 1. / ((rnd->camera->plane.x * rnd->camera->dir.y) - (rnd->camera->plane.y * rnd->camera->dir.x));
	sorted = sort_sprites(rnd->camera, sprites);
	while (sorted) {
		if (sorted->distance > .1) {
			init_draw_sprite(rnd, sorted, inv_det, &spr);
			draw_sprite(rnd, sorted, &spr, sorted->tex);
		}
		sorted = sorted->sorted;
	}
}

/* ************************************************************************** */
// スプライト描画用の座標計算と初期化を行う
static void
	init_draw_sprite(t_render* rnd, t_sprite* sprite, double inv_det, t_sprite_draw* spr)
{
	set_pos(&spr->pos, sprite->pos.x - rnd->camera->pos.x, sprite->pos.y - rnd->camera->pos.y);
	set_pos(&spr->transform,
		inv_det * (rnd->camera->dir.y * spr->pos.x - rnd->camera->dir.x * spr->pos.y),
		inv_det * (-rnd->camera->plane.y * spr->pos.x + rnd->camera->plane.x * spr->pos.y));
	spr->sprite_screen = (int)((rnd->w->size.x / 2.) * (1. + spr->transform.x / spr->transform.y));
	spr->spr_s.x = fabs(rnd->w->size.y / spr->transform.y);
	spr->spr_s.y = fabs(rnd->w->size.y / spr->transform.y);
	set_pos(&spr->draw_x,
		(int)(MAX(0, -spr->spr_s.x / 2. + spr->sprite_screen)),
		(int)(MAX(0, spr->spr_s.x / 2. + spr->sprite_screen)));
	set_pos(&spr->draw_y,
		(int)(MAX(0, -spr->spr_s.y / 2. + rnd->w->size.y / 2.)),
		(int)(MAX(0, spr->spr_s.y / 2. + rnd->w->size.y / 2.)));
	spr->draw_y_org = spr->draw_y.x;
}

/* ************************************************************************** */
// スプライト全体を画面に描画する
static void
	draw_sprite(t_render* rnd, t_sprite* sprite, t_sprite_draw* spr, t_tex* tex)
{
	while (spr->draw_x.x < rnd->w->size.x && spr->draw_x.x < spr->draw_x.y) {
		if (spr->transform.y > 0. && spr->transform.y < rnd->depth[(int)spr->draw_x.x]) {
			spr->draw_y.x = spr->draw_y_org;
			while (spr->draw_y.x < rnd->w->size.y && spr->draw_y.x < spr->draw_y.y) {
				draw_sprite_pixel(rnd, sprite, spr, tex);
				spr->draw_y.x++;
			}
		}
		spr->draw_x.x++;
	}
}

/* ************************************************************************** */
// スプライトの1ピクセルを描画する
static void
	draw_sprite_pixel(t_render* rnd, t_sprite* sprite, t_sprite_draw* spr, t_tex* tex)
{
	t_pos	pixel;
	t_pos	tex_pos;
	int		color;

	set_pos(&pixel, spr->draw_x.x, spr->draw_y.x);
	if (set_tex_pos(rnd, spr, tex, &tex_pos)) {
		color = shade_color(get_tex_color(tex, &tex_pos), (rnd->options & FLAG_SHADOWS) ? sprite->distance / 3 : 1);
		if (color != 0x0) {
			draw_pixel(rnd->w, &pixel, color);
		}
	}
}

/* ************************************************************************** */
// スプライトのテクスチャ座標を計算する
static int
	set_tex_pos(t_render* rnd, t_sprite_draw* spr, t_tex* tex, t_pos* tex_pos)
{
	tex_pos->x = (int)(256 * (((int)(spr->draw_x.x) - (-spr->spr_s.x / 2. + spr->sprite_screen))) * tex->width / spr->spr_s.x) / 256;
	if (tex_pos->x < tex->start.x || tex_pos->x > tex->end.x) {
		return (0);
	}
	spr->fact = ((int)(spr->draw_y.x) * 256.) - (rnd->w->size.y * 128.) + (spr->spr_s.y * 128.);
	tex_pos->y = ((spr->fact * tex->height) / spr->spr_s.y) / 256.;
	return (tex_pos->y > tex->start.y && tex_pos->y < tex->end.y);
}
