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
// スプライト全体を画面に描画する（最適化版：ループ不変量の外出し＋ステップ加算）
static void
	draw_sprite(t_render* rnd, t_sprite* sprite, t_sprite_draw* spr, t_tex* tex)
{
	double	step_y;
	double	tex_pos_y_start;
	double	tex_pos_y;
	t_pos	tex_pos;
	t_pos	pixel;
	int		color;

	/* Y方向の1ピクセルあたりのテクスチャ進行度（ステップ）を事前計算 */
	step_y = 1.0 * tex->height / spr->spr_s.y;
	/* Y方向のテクスチャ開始座標 */
	tex_pos_y_start = (spr->draw_y_org - rnd->w->size.y / 2.0 + spr->spr_s.y / 2.0) * step_y;

	while (spr->draw_x.x < rnd->w->size.x && spr->draw_x.x < spr->draw_x.y) {
		/* Zバッファ（depth）を用いた遮蔽判定 */
		if (spr->transform.y > 0. && spr->transform.y < rnd->depth[(int)spr->draw_x.x]) {
			
			/* X方向のテクスチャ座標はYループ内で一定なため、外側で1度だけ計算する */
			tex_pos.x = (int)(256 * (((int)(spr->draw_x.x) - (-spr->spr_s.x / 2. + spr->sprite_screen))) * tex->width / spr->spr_s.x) / 256;
			
			/* X座標が有効なテクスチャ範囲内の場合のみYループを回す（無駄なループを排除） */
			if (tex_pos.x >= tex->start.x && tex_pos.x <= tex->end.x) {
				spr->draw_y.x = spr->draw_y_org;
				tex_pos_y = tex_pos_y_start;
				pixel.x = spr->draw_x.x;
				
				while (spr->draw_y.x < rnd->w->size.y && spr->draw_y.x < spr->draw_y.y) {
					tex_pos.y = (int)tex_pos_y;
					
					/* テクスチャ範囲の超過を防ぐ安全対策 */
					if (tex_pos.y >= tex->height) {
						tex_pos.y = tex->height - 1;
					} else if (tex_pos.y < 0) {
						tex_pos.y = 0;
					}

					if (tex_pos.y > tex->start.y && tex_pos.y < tex->end.y) {
						color = shade_color(get_tex_color(tex, &tex_pos), (rnd->options & FLAG_SHADOWS) ? sprite->distance / 3 : 1);
						/* 下位24ビット(RGB成分)だけを取り出し、0(黒)でなければ描画する */
						if ((color & 0x00FFFFFF)) {
							pixel.y = spr->draw_y.x;
							draw_pixel(rnd->w, &pixel, color);
						}
					}
					
					/* ループ内は重い乗除算を排除し、加算のみでテクスチャ座標を進める */
					tex_pos_y += step_y;
					spr->draw_y.x++;
				}
			}
		}
		spr->draw_x.x++;
	}
}
