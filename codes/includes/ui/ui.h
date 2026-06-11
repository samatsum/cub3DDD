#ifndef UI_H
# define UI_H

# include "engine/render/render.h"

/* ************************************************************************** */
// ミニマップの描画やUIの座標・サイズ設定
# define MAP_TILE_SIZE			5
# define SCALE					10
# define UI_BG_X				2
# define UI_BG_Y				27
# define UI_BG_SIZE				210
# define UI_TEXT_Y				5
# define UI_BUF_SIZE			100

// UIやミニマップの各要素の色定義
# define COLOR_MINIMAP_WALL		0x000000
# define COLOR_MINIMAP_BG		0x10CC10
# define COLOR_MINIMAP_EMPTY	0xFFFFFF
# define COLOR_UI_BG			0xFFFFFF
# define COLOR_UI_TEXT			0xCCCCCC
# define COLOR_UI_FONT			0x000000

// 照準（クロスヘア）の描画設定
# define CH_OUTER				11
# define CH_LENGTH				10
# define CH_GAP					4
# define COLOR_CROSSHAIR		0xA020F0

/* ************************************************************************** */
void
	update_ui(t_render* rnd);
void
	write_ui_text(t_window* w, int collected, int to_collect);
void
	display_crosshair(t_window* w);

#endif
