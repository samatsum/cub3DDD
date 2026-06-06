#include "engine/render/render.h"
#include "engine/texture/texture.h"
#include "core/core.h"

/* ************************************************************************** */
void
	draw_weapon(t_game* game);

/* ************************************************************************** */
// 画面下部中央に武器のテクスチャをオーバレイ描画する
void
	draw_weapon(t_game* game)
{
	t_pos	pixel;
	t_pos	p_tex;
	int		color;
	double	scale;
	t_pos	start;
	t_tex*	active_tex;

	/* 状態に合わせて描画するテクスチャを選択 */
	if (game->current_weapon == WEP_PISTOL) {
		if (game->is_shooting)
			active_tex = &game->weapon_tex[1]; /* 射撃中 */
		else
			active_tex = &game->weapon_tex[0]; /* 待機中 */
	} else {
		active_tex = &game->weapon_tex[2];     /* 懐中電灯 */
	}

	if (!active_tex->tex)
		return ;

	scale = (game->window.size.y * 0.6) / active_tex->height;
	start.x = (game->window.size.x / 2.0) - ((active_tex->width * scale) / 2.0);
	start.y = game->window.size.y - (active_tex->height * scale);

	pixel.y = start.y;
	while (pixel.y < game->window.size.y)
	{
		pixel.x = start.x;
		while (pixel.x < start.x + (active_tex->width * scale))
		{
			p_tex.x = (int)((pixel.x - start.x) / scale);
			p_tex.y = (int)((pixel.y - start.y) / scale);

			if (p_tex.x >= 0 && p_tex.x < active_tex->width &&
				p_tex.y >= 0 && p_tex.y < active_tex->height)
			{
				color = get_tex_color(active_tex, &p_tex);
				
				/* 透過判定（黒色をスキップ） */
				if ((color & 0x00FFFFFF) != 0x000000)
				{
					draw_pixel(&game->window, &pixel, color);
				}
			}
			pixel.x++;
		}
		pixel.y++;
	}
}
