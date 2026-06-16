#include "config/config.h"
#include "engine/render/render.h"
#include "engine/texture/texture.h" /* get_tex_color, distance_shade 等を使うため */

/* ************************************************************************** */

void
	draw_wall(t_render* rnd, t_raysult* r);
static void
	init_draw_wall(t_tex* tex, t_raysult* ray, t_pos* p_tex);
static void
	draw_textured_column(t_render* rnd, t_raysult* r, t_tex* tex, t_pos* p_tex, double light);

/* ************************************************************************** */

// 1列分の壁を描画する
void
	draw_wall(t_render* rnd, t_raysult* r)
{
	t_pos	p_tex;
	t_pos	pixel;
	t_tex*	tex;
	double	light;

	if (r->is_door) {
		tex = rnd->door_tex;
	} else {
		tex = &rnd->tex[r->direction];
	}
	light = flashlight_weight(rnd, r->column);
	if (!tex->tex) {
		set_pos(&pixel, r->column, MAX(0, rnd->w->half.y - (r->height / 2.)));
		draw_vertical_line(rnd->w, &pixel, r->height,
			distance_shade(rnd->options, rnd->config->colors[r->direction], r->distance, light));
		return ;
	}
	init_draw_wall(tex, r, &p_tex);
	draw_textured_column(rnd, r, tex, &p_tex, light);
}

/* ************************************************************************** */

// 壁描画のためのテクスチャX座標を計算する
static void
	init_draw_wall(t_tex* tex, t_raysult* ray, t_pos* p_tex)
{
	if (ray->side) {
		ray->wall_x = ray->ray_pos.x + ((ray->map_pos.y - ray->ray_pos.y + (1. - ray->step.y) / 2.) / ray->ray_dir.y) * ray->ray_dir.x;
	} else {
		ray->wall_x = ray->ray_pos.y + ((ray->map_pos.x - ray->ray_pos.x + (1. - ray->step.x) / 2.) / ray->ray_dir.x) * ray->ray_dir.y;
	}
	ray->wall_x -= floor(ray->wall_x);
	p_tex->x = (int)(ray->wall_x * tex->width);
	if (ray->side == 0 && ray->ray_dir.x > 0.) {
		p_tex->x = tex->width - p_tex->x - 1.;
	} else if (ray->side == 1 && ray->ray_dir.y < 0.) {
		p_tex->x = tex->width - p_tex->x - 1.;
	}
}

/* ************************************************************************** */

// テクスチャ付き壁を1列ぶん、境界チェック無しでバックバッファへ直接書き込む。
// 列xは画面内に必ず収まり、yも開始(start_y>=0)と終端(end)で挟むため、draw_pixelの
// 毎ピクセル境界判定とdouble→int変換を省ける。dstを行ストライド刻みで縦に進める
static void
	draw_textured_column(t_render* rnd, t_raysult* r, t_tex* tex, t_pos* p_tex, double light)
{
	unsigned int*	dst;
	unsigned int*	end;
	int				stride;
	int				start_y;
	int				i;
	double			step;
	double			tex_pos;

	stride = rnd->w->screen.size_line / 4;
	start_y = (int)MAX(0, rnd->w->half.y - (r->height / 2.));
	/* 1ピクセル描画ごとのテクスチャY座標の増加量（ステップ値）を事前計算 */
	step = 1.0 * tex->height / r->height;
	/* テクスチャY座標の初期値を計算（壁が画面外にはみ出している場合を考慮） */
	tex_pos = (start_y - rnd->w->size.y / 2.0 + r->height / 2.0) * step;
	/* 列の先頭ポインタと、この列の終端（画面下端）を確定する */
	dst = (unsigned int*)rnd->w->screen.ptr + (int)r->column + start_y * stride;
	end = (unsigned int*)rnd->w->screen.ptr + (int)r->column + (int)rnd->w->size.y * stride;
	i = 0;
	while (i < r->height && dst < end) {
		p_tex->y = (int)tex_pos;
		/* テクスチャサイズの超過を防ぐ安全対策 */
		if (p_tex->y >= tex->height) {
			p_tex->y = tex->height - 1;
		} else if (p_tex->y < 0) {
			p_tex->y = 0;
		}
		/* 境界判定済みなので draw_pixel を介さず直接書き込む */
		*dst = (unsigned int)distance_shade(rnd->options, get_tex_color(tex, p_tex), r->distance, light);
		/* ループ内では浮動小数点の乗算を排除し、加算とポインタ前進のみで更新 */
		dst += stride;
		tex_pos += step;
		i++;
	}
}
