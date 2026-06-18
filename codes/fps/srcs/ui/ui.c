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
static void
	draw_entity_dot(t_render* rnd, t_pos* pos, int color, int tile, int margin);
static int
	case_color(t_config* config, int mode, int x, int y);
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
// 画面右下にミニマップを描画する（マス目描画後、実座標でエンティティを上書き）
static void
	draw_minimap(t_render* rnd, t_pos* start, t_pos* end)
{
	int			i;
	int			j;
	int			color;
	int			tile;
	int			margin;
	int			base_x;
	int			base_y;
	t_enemy*	e;

	tile = scale_ui_px(MAP_TILE_SIZE, rnd->w->size.y);
	margin = scale_ui_px(SCALE, rnd->w->size.y);
	
	// ミニマップ全体を描画する基準点（左上座標）。画面右端・下端から margin 分の余白を空ける
	base_x = rnd->w->size.x - margin - (rnd->config->map.columns * tile);
	base_y = rnd->w->size.y - margin - (rnd->config->map.rows * tile);

	// 1. 背景のマス（壁・床など）をタイル単位で描画
	i = 0;
	while (i < rnd->config->map.rows) {
		j = 0;
		while (j < rnd->config->map.columns) {
			color = case_color(rnd->config, rnd->mode, j, i);
			// マップ外の空白(-1)でない場合のみ描画する
			if (color >= 0) {
				set_pos(start, base_x + (j * tile), base_y + (i * tile));
				set_pos(end, start->x + tile, start->y + tile); // 幅と高さを厳密に tile に設定
				draw_rectangle(rnd->w, start, end, color);
			}
			j++;
		}
		i++;
	}

	// 2. 敵を実座標に基づいて描画
	e = rnd->world->enemies;
	while (e) {
		draw_entity_dot(rnd, &e->sprite->pos, COLOR_MINIMAP_ENEMY, tile, margin);
		e = e->next;
	}

	// 3. プレイヤーを実座標に基づいて描画（一番手前に表示するため最後に描画）
	draw_entity_dot(rnd, &rnd->camera->pos, COLOR_MINIMAP_BG, tile, margin);
}

/* ************************************************************************** */
// エンティティを実座標ベースでミニマップ上に描画する（サイズはマスの60%に縮小）
static void
	draw_entity_dot(t_render* rnd, t_pos* pos, int color, int tile, int margin)
{
	t_pos	start;
	t_pos	end;
	double	cx;
	double	cy;
	double	dot_size;
	double	base_x;
	double	base_y;

	// ミニマップの基準点（左上）を計算
	base_x = rnd->w->size.x - margin - (rnd->config->map.columns * tile);
	base_y = rnd->w->size.y - margin - (rnd->config->map.rows * tile);

	// 実座標(pos->x, pos->y)を画面上のピクセル座標に変換
	cx = base_x + (pos->x * tile);
	cy = base_y + (pos->y * tile);
	
	// ドットのサイズをマスの60%とする
	dot_size = tile * 0.6;
	if (dot_size < 2.0) {
		dot_size = 2.0;
	}
	set_pos(&start, cx - (dot_size / 2.0), cy - (dot_size / 2.0));
	set_pos(&end, cx + (dot_size / 2.0), cy + (dot_size / 2.0));
	draw_rectangle(rnd->w, &start, &end, color);
}

/* ************************************************************************** */
// ミニマップの特定マスにおける背景色を判定して返す
static int
	case_color(t_config* config, int mode, int x, int y)
{
	char	c;

	c = MAP_XY(x, y, *config);

	// マップ外の空白領域（スペース等）は描画せず透明にする
	if (c == ' ' || c == '\0') {
		return (-1);
	}

	if (IS_BLOCKING(c)) {
		return (COLOR_MINIMAP_WALL);
	}
	// リスポーン地点(N/S/E/W)はチーム別に塗る。ただしFPSモードはチームがないため
	// 全リスポーン地点を青で統一する
	if (IS_SPAWN(c)) {
		if (mode == MODE_FPS || IS_BLUE_SPAWN(c)) {
			return (COLOR_MINIMAP_SPAWN_BLUE);
		}
		return (COLOR_MINIMAP_SPAWN_RED);
	}
	if (c == 'A') {
		return (COLOR_UI_TEXT);
	}
	// '0'（床）など、実際に存在する通路は背景色で塗る
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
