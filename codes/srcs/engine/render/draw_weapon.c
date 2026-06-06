#include <sys/time.h>
#include <math.h>
#include "engine/render/render.h"
#include "engine/texture/texture.h"
#include "core/core.h"

/* ************************************************************************** */
void
	draw_weapon(t_game* game);
static void
	draw_overlay(t_game* game, t_tex* tex, double start_x, double start_y, double scale);

/* ************************************************************************** */
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
	static long long	last_update = 0;
	struct timeval		tv;
	long long			current_time;
	t_tex*				active_tex;
	double				scale;
	double				start_x;
	double				start_y;
	double				angle;
	double				move_dist;
	int					is_moving;

	gettimeofday(&tv, NULL);
	current_time = (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000);
	if (game->is_shooting > 0 && (current_time - last_update) >= 100) {
		game->is_shooting--;
		last_update = current_time;
	}
	is_moving = game->move.x || game->move.y || game->x_move.x || game->x_move.y;
	if (game->current_weapon == WEP_HANDS) {
		if (!is_moving) {
			return ;
		}
		/* 2秒周期 */
		angle = (double)(current_time % 2000) / 2000.0 * 2.0 * M_PI;
		
		/* * 修正: 
		 * fabs(sin(angle)) で 0(端)〜1(中心)〜0(端) の波形を作り、
		 * それを 5乗することで「端ではほぼ止まり、中心で一気に加速する」動きにします。
		 */
		double bob_left = pow(fabs(sin(angle)), 3.0);
		double bob_right = pow(fabs(cos(angle)), 3.0);
		
		/* 左手 */
		active_tex = &game->weapon_tex[4];
		if (active_tex->tex) {
			scale = (game->window.size.y * 0.6) / active_tex->height;
			move_dist = active_tex->height * scale * 0.8;
			
			/* 端(0)で基準位置、中心(1)で最大移動 */
			start_x = 0 - move_dist * bob_left;
			start_y = game->window.size.y - (active_tex->height * scale) + move_dist * bob_left;
			draw_overlay(game, active_tex, start_x, start_y, scale);
		}
		
		/* 右手 */
		active_tex = &game->weapon_tex[5];
		if (active_tex->tex) {
			scale = (game->window.size.y * 0.6) / active_tex->height;
			move_dist = active_tex->height * scale * 0.8;
			
			/* 右手も同様に cos の絶対値を5乗してタメを作る */
			start_x = game->window.size.x - (active_tex->width * scale) + move_dist * bob_right;
			start_y = game->window.size.y - (active_tex->height * scale) + move_dist * bob_right;
			draw_overlay(game, active_tex, start_x, start_y, scale);
		}
		return ;
	}
	/* ピストル または フラッシュライトの描画 */
	if (game->current_weapon == WEP_PISTOL) {
		if (game->is_shooting > 7) active_tex = &game->weapon_tex[1];
		else if (game->is_shooting > 0) active_tex = &game->weapon_tex[2];
		else active_tex = &game->weapon_tex[0];
	} else active_tex = &game->weapon_tex[3];
	if (!active_tex->tex) return ;
	scale = (game->window.size.y * 0.6) / active_tex->height;
	start_x = (game->window.size.x / 2.0) - ((active_tex->width * scale) / 2.0);
	start_y = game->window.size.y - (active_tex->height * scale);
	draw_overlay(game, active_tex, start_x, start_y, scale);
}
