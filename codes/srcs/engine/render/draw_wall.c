#include "config/config.h"
#include "engine/render/render.h"
#include "engine/texture/texture.h" /* get_tex_color, distance_shade 等を使うため */

/* ************************************************************************** */

void
	draw_wall(t_render* rnd, t_raysult* r);
static void
	init_draw_wall(t_tex* tex, t_raysult* ray, t_pos* p_tex);

/* ************************************************************************** */

// 1列分の壁を描画する
void
	draw_wall(t_render* rnd, t_raysult* r)
{
	t_pos	p_tex;
	t_pos	pixel;
	t_tex*	tex;
	int		i[2];
	double	step;
	double	tex_pos;
	double	light;

	tex = &rnd->tex[r->direction];
	light = flashlight_weight(rnd, r->column);
	set_pos(&pixel, r->column, MAX(0, rnd->w->half.y - (r->height / 2.)));
	if (!tex->tex) {
		draw_vertical_line(rnd->w, &pixel, r->height,
			distance_shade(rnd->options, rnd->config->colors[r->direction], r->distance, light));
		return ;
	}
	init_draw_wall(tex, r, &p_tex);
	i[1] = MAX(0, rnd->w->half.y - (r->height / 2.));
	i[0] = 0;
	/* 1ピクセル描画ごとのテクスチャY座標の増加量（ステップ値）を事前計算 */
	step = 1.0 * tex->height / r->height;
	/* テクスチャY座標の初期値を計算（壁が画面外にはみ出している場合を考慮） */
	tex_pos = (i[1] - rnd->w->size.y / 2.0 + r->height / 2.0) * step;
	while (i[0] < r->height && (pixel.y = i[1]++) < rnd->w->size.y) {
		p_tex.y = (int)tex_pos;
		/* テクスチャサイズの超過を防ぐ安全対策 */
		if (p_tex.y >= tex->height) {
			p_tex.y = tex->height - 1;
		} else if (p_tex.y < 0) {
			p_tex.y = 0;
		}
		draw_pixel(rnd->w, &pixel, distance_shade(rnd->options, get_tex_color(tex, &p_tex), r->distance, light));
		/* ループ内では浮動小数点の乗算を排除し、加算のみで更新 */
		tex_pos += step;
		i[0]++;
	}
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
