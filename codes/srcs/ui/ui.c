#include "ui/ui.h"
#include "ui/font.h"
#include "config/config.h"
#include "engine/render/render.h"
#include "utils/utils.h"
#include "tuning.h"

/* ************************************************************************** */
void
	update_ui(t_render* rnd);
void
	write_ui_text(t_window* w, int collected, int to_collect);
static void
	build_status_text(char* buf, int collected, int to_collect);
static void
	draw_minimap(t_render* rnd, t_pos* start, t_pos* end);
static int
	case_color(t_config* config, t_camera* camera, int x, int y);
static int
	scale_ui_px(int base, int win_height);

/* ************************************************************************** */
// UIの背景とミニマップを描画する（欄・タイルは解像度に比例して拡大）
void
	update_ui(t_render* rnd)
{
	t_pos	start;
	t_pos	end;
	int		bg_x;
	int		bg_y;
	int		bg_size;

	bg_x = scale_ui_px(UI_BG_X, rnd->w->size.y);
	bg_y = scale_ui_px(UI_BG_Y, rnd->w->size.y);
	bg_size = scale_ui_px(UI_BG_SIZE, rnd->w->size.y);
	set_pos(&start, bg_x, rnd->w->size.y - bg_y);
	set_pos(&end, bg_size, rnd->w->size.y - bg_x);
	draw_rectangle(rnd->w, &start, &end, COLOR_UI_BG);
	draw_minimap(rnd, &start, &end);
}

/* ************************************************************************** */
// 収集状況のテキストを自前フォントで描画する（位置・サイズは解像度連動）
void
	write_ui_text(t_window* w, int collected, int to_collect)
{
	char	buf[UI_BUF_SIZE];
	t_pos	start;
	int		scale;
	int		box_top;
	int		box_bot;

	build_status_text(buf, collected, to_collect);
	scale = scale_ui_px(UI_TEXT_SCALE, w->size.y);
	box_top = w->size.y - scale_ui_px(UI_BG_Y, w->size.y);
	box_bot = w->size.y - scale_ui_px(UI_BG_X, w->size.y);
	set_pos(&start, scale_ui_px(UI_BG_X + UI_TEXT_PAD, w->size.y),
		box_top + (box_bot - box_top - FONT_H * scale) / 2);
	draw_text_scaled(w, &start, buf, scale, COLOR_UI_FONT);
}

/* ************************************************************************** */
// 収集状況を表す文字列を buf に組み立てる
static void
	build_status_text(char* buf, int collected, int to_collect)
{
	int	i;

	i = 0;
	while (i < UI_BUF_SIZE) {
		buf[i++] = 0;
	}
	if (to_collect > 0 && to_collect == collected) {
		ft_write_str(buf, "ALL COLLECTED!", 0);
	} else if (to_collect > 0) {
		i = ft_write_str(buf, "Collect: ", 0);
		i = ft_write_str(buf, " / ", ft_write_int(buf, collected, i));
		i = ft_write_int(buf, to_collect, i);
	} else {
		ft_write_str(buf, "Nothing to collect !", 0);
	}
}

/* ************************************************************************** */
// 画面右下にミニマップを描画する（タイル・余白は解像度に比例して拡大する）
static void
	draw_minimap(t_render* rnd, t_pos* start, t_pos* end)
{
	int	i;
	int	j;
	int	color;
	int	tile;
	int	margin;

	tile = scale_ui_px(MAP_TILE_SIZE, rnd->w->size.y);
	margin = scale_ui_px(SCALE, rnd->w->size.y);
	i = 0;
	while (i < rnd->config->map.rows) {
		j = 0;
		while (j < rnd->config->map.columns) {
			color = case_color(rnd->config, rnd->camera, j, i);
			if (color >= 0) {
				set_pos(start,
					rnd->w->size.x - (rnd->config->map.columns * tile) - margin + (j * tile),
					rnd->w->size.y - (rnd->config->map.rows * tile) - margin + (i * tile));
				set_pos(end,
					rnd->w->size.x - (rnd->config->map.columns * tile) + (j * tile),
					rnd->w->size.y - (rnd->config->map.rows * tile) + (i * tile));
				draw_rectangle(rnd->w, start, end, color);
			}
			j++;
		}
		i++;
	}
}

/* ************************************************************************** */
// ミニマップの特定座標における色を判定して返す
static int
	case_color(t_config* config, t_camera* camera, int x, int y)
{
	char	c;

	c = MAP_XY(x, y, *config);
	if (IS_BLOCKING(c)) {
		return (COLOR_MINIMAP_WALL);
	} else if (y == (int)camera->pos.y && x == (int)camera->pos.x) {
		return (COLOR_MINIMAP_BG);
	} else if (c == 'A') {
		return (COLOR_UI_TEXT);
	}
	return (COLOR_MINIMAP_EMPTY);
}

/* ************************************************************************** */
// 基準解像度(UI_REF_HEIGHT)で base px の量を、実解像度に比例させて返す
static int
	scale_ui_px(int base, int win_height)
{
	int	scaled;

	scaled = base * win_height / UI_REF_HEIGHT;
	if (scaled < base) {
		scaled = base;
	}
	return (scaled);
}
