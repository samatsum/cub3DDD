#include "core/core.h"            /* t_game 定義のため */
#include "core/respawn.h"         /* is_player_dead に必要 */
#include "engine/raycast/raycast.h"
#include "engine/render/render.h" /* 描画関数群の呼び出しに必要 */
#include "ui/ui.h"                /* update_ui, display_crosshair, write_ui_text に必要 */
#include "ui/font.h"              /* draw_text_scaled / FONT_* に必要 */
#include "rsp/rsp_game.h"          /* RSP_SCORE_LIMIT 用 */
#include "tuning.h"               /* LIGHT_CONE_DEG に必要 */
#include "../minilibx-linux/mlx.h" /* mlx_put_image_to_window 用 */

/* ************************************************************************** */
void
	render_frame(t_game* game);
static void
	update_screen(t_game* game);
static void
	update_window(t_game* game);
static void
	draw_death_screen(t_game* game);
static void
	draw_clear_screen(t_game* game);
static void
	build_clear_text(char* buf, long long elapsed_ms);
static int
	write_two_digits(char* buf, int val, int start);
static void
	draw_centered_text(t_window* w, char* text, int y, int scale);
static void
	build_rsp_score_text(t_game* game, char* buf);

/* ************************************************************************** */
// 1フレーム全体を描画する。死亡中(is_player_dead)はワールドもUIも伏せ、死亡画像だけを描いて
// 即ウィンドウへ転送する。通常時は update_screen で世界を組み立て、update_window で反映する
void
	render_frame(t_game* game)
{
	if (game->cleared) {
		draw_clear_screen(game);
		mlx_put_image_to_window(game->window.ptr, game->window.win, game->window.screen.img, 0, 0);
		return ;
	}
	if (is_player_dead(game)) {
		draw_death_screen(game);
		mlx_put_image_to_window(game->window.ptr, game->window.win, game->window.screen.img, 0, 0);
		return ;
	}
	update_screen(game);
	update_window(game);
}

/* ************************************************************************** */
// 画面を黒で消去し、描画に必要な状態を t_render スナップショットへ集約してから、列群を並列
// レイキャストする(cast_columns)。その上にスプライト→武器→照準→UIの順で重ねる。
// 懐中電灯を装備中ならフラッシュライトのフラグを立て、各描画の暗化補正に反映させる
static void
	update_screen(t_game* game)
{
	t_render	rnd;
	t_window*	w;
	t_pos		start;

	w = &game->window;
	set_pos(&start, 0, 0);
	draw_rectangle(w, &start, &w->size, 0x0);
	rnd.w = w;
	rnd.config = &game->config;
	rnd.camera = &game->camera;
	rnd.world = &game->world;
	rnd.tex = game->assets.tex;
	rnd.door_tex = &game->assets.door_tex;
	rnd.depth = game->cache.depth;
	rnd.sf_dist = game->cache.sf_dist;
	rnd.options = game->options;
	rnd.mode = game->mode;
	if (game->input.current_weapon == WEP_FLASHLIGHT) {
		rnd.options = rnd.options | FLAG_FLASHLIGHT;
	}
	cast_columns(&rnd, game->cache.camera_x);
	if (game->world.sprites) {
		draw_sprites(&rnd, game->world.sprites);
	}
	draw_weapon(game);
	if (game->options & FLAG_CROSSHAIR) {
		display_crosshair(w);
	}
	if (game->options & FLAG_UI) {
		update_ui(&rnd);
	}
}

/* ************************************************************************** */
// 完成したバッファ画像をウィンドウへ転送する。UIが有効なら、転送前に収集状況のテキストを
// 画面へ書き込んでおく
static void
	update_window(t_game* game)
{
	if (game->options & FLAG_UI) {
		write_ui_text(game);
	}
	mlx_put_image_to_window(game->window.ptr, game->window.win, game->window.screen.img, 0, 0);
}

/* ************************************************************************** */
// 死亡演出。バッファを黒で塗ってから死亡画像を画面全体へ最近傍補間で引き伸ばす。各画面ピクセル
// (pixel)に対応するテクセル(texel)を比率で求めてサンプリングする。画像未ロード時は黒画面のまま
static void
	draw_death_screen(t_game* game)
{
	t_window*	w;
	t_tex*		tex;
	t_pos		pixel;
	t_pos		texel;

	w = &game->window;
	set_pos(&pixel, 0, 0);
	draw_rectangle(w, &pixel, &w->size, 0x000000);
	tex = &game->assets.death_tex;
	if (!tex->tex) {
		return ;
	}
	pixel.y = 0;
	while (pixel.y < w->size.y) {
		texel.y = (int)(pixel.y * tex->height / w->size.y);
		pixel.x = 0;
		while (pixel.x < w->size.x) {
			texel.x = (int)(pixel.x * tex->width / w->size.x);
			draw_pixel(w, &pixel, get_tex_color(tex, &texel));
			pixel.x++;
		}
		pixel.x = 0;
		pixel.y++;
	}
}


/* ************************************************************************** */
// クリア画面。黒背景に、開始からゴール到達までの経過時間（分:秒）を中央へ描画する
static void
	draw_clear_screen(t_game* game)
{
	t_window*	w;
	t_pos		start;
	char		buf[UI_BUF_SIZE];
	int			scale;
	int			text_w;
	int			text_h;

	w = &game->window;
	set_pos(&start, 0, 0);
	draw_rectangle(w, &start, &w->size, 0x000000);
	scale = w->size.y / 160;
	if (scale < 3) {
		scale = 3;
	}
	if (game->mode == MODE_RSP) {
		if (game->rsp_winner == (int)game->player_rsp.team) {
			ft_write_str(buf, "VICTORY", 0);
		} else {
			ft_write_str(buf, "DEFEAT", 0);
		}
		draw_centered_text(w, buf, (w->size.y / 2) - (FONT_H * scale), scale);
		build_rsp_score_text(game, buf);
		draw_centered_text(w, buf, (w->size.y / 2) + (FONT_H * scale), scale);
		return ;
	}
	build_clear_text(buf, game->clear_time_ms);
	text_w = ft_strlen(buf) * FONT_W * scale;
	text_h = FONT_H * scale;
	set_pos(&start, (w->size.x - text_w) / 2, (w->size.y - text_h) / 2);
	draw_text_scaled(w, &start, buf, scale, 0xFFFFFF);
}

/* ************************************************************************** */
// "CLEAR TIME Mm SSs" の文字列を組み立てる
static void
	build_clear_text(char* buf, long long elapsed_ms)
{
	int	i;
	int	total_seconds;
	int	minutes;
	int	seconds;

	i = 0;
	while (i < UI_BUF_SIZE) {
		buf[i++] = 0;
	}
	total_seconds = (int)(elapsed_ms / 1000);
	minutes = total_seconds / 60;
	seconds = total_seconds % 60;
	i = ft_write_str(buf, "CLEAR TIME ", 0);
	i = ft_write_int(buf, minutes, i);
	i = ft_write_str(buf, "m ", i);
	i = write_two_digits(buf, seconds, i);
	ft_write_str(buf, "s", i);
}

/* ************************************************************************** */
// 0〜99 を2桁で書き込む。秒表示の 00〜59 に使う
static int
	write_two_digits(char* buf, int val, int start)
{
	buf[start++] = '0' + ((val / 10) % 10);
	buf[start++] = '0' + (val % 10);
	buf[start] = 0;
	return (start);
}


/* ************************************************************************** */
// 文字列を指定 y 座標で画面中央に描画する
static void
	draw_centered_text(t_window* w, char* text, int y, int scale)
{
	t_pos	start;
	int		text_w;

	text_w = ft_strlen(text) * FONT_W * scale;
	set_pos(&start, (w->size.x - text_w) / 2, y);
	draw_text_scaled(w, &start, text, scale, 0xFFFFFF);
}

/* ************************************************************************** */
// RSPスコア表示文字列を組み立てる
static void
	build_rsp_score_text(t_game* game, char* buf)
{
	int	i;

	i = 0;
	while (i < UI_BUF_SIZE) {
		buf[i++] = 0;
	}
	i = ft_write_str(buf, "Red ", 0);
	i = ft_write_int(buf, game->rsp_score[TEAM_RED], i);
	i = ft_write_str(buf, "/", i);
	i = ft_write_int(buf, RSP_SCORE_LIMIT, i);
	i = ft_write_str(buf, "  VS  Blue ", i);
	i = ft_write_int(buf, game->rsp_score[TEAM_BLUE], i);
	i = ft_write_str(buf, "/", i);
	ft_write_int(buf, RSP_SCORE_LIMIT, i);
}
