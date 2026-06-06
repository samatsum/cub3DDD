#include <sys/time.h>
#include <stdio.h>      /* 追加: printf 用 */
#include "engine/render/render.h"
#include "engine/texture/texture.h"
#include "core/core.h"

/* ************************************************************************** */
void
	draw_weapon(t_game* game);

/* ************************************************************************** */
void
	draw_weapon(t_game* game)
{
	t_pos			pixel;
	t_pos			p_tex;
	t_pos			start;
	t_tex*			active_tex;
	int				color;
	double			scale;
	struct timeval	tv;
	long long		current_time;
	static long long last_update = 0;

	gettimeofday(&tv, NULL);
	current_time = (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000);

	/* アニメーションを確実に視認できるよう、100ミリ秒(0.1秒)間隔に延長 */
	if (game->is_shooting > 0 && (current_time - last_update) >= 100) {
		game->is_shooting--;
		last_update = current_time;
		/* 状態変化をターミナルに出力 */
		// printf("Timer decremented: %d\n", game->is_shooting);
	}

	if (game->current_weapon == WEP_PISTOL) {
		if (game->is_shooting > 7) {
			active_tex = &game->weapon_tex[1];
		} else if (game->is_shooting > 0) {
			active_tex = &game->weapon_tex[2];
		} else {
			active_tex = &game->weapon_tex[0];
		}
	} else {
		active_tex = &game->weapon_tex[3];
	}

	if (!active_tex->tex) {
		return ;
	}

	scale = (game->window.size.y * 0.6) / active_tex->height;
	start.x = (game->window.size.x / 2.0) - ((active_tex->width * scale) / 2.0);
	start.y = game->window.size.y - (active_tex->height * scale);

	pixel.y = start.y;
	while (pixel.y < game->window.size.y) {
		pixel.x = start.x;
		while (pixel.x < start.x + (active_tex->width * scale)) {
			p_tex.x = (int)((pixel.x - start.x) / scale);
			p_tex.y = (int)((pixel.y - start.y) / scale);

			if (p_tex.x >= 0 && p_tex.x < active_tex->width && p_tex.y >= 0 && p_tex.y < active_tex->height) {
				color = get_tex_color(active_tex, &p_tex);
				if ((color & 0x00FFFFFF) != 0x000000) {
					draw_pixel(&game->window, &pixel, color);
				}
			}
			pixel.x++;
		}
		pixel.y++;
	}
}
