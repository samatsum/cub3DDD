#include <time.h>   /* time, localtime用 */
#include <stdlib.h> /* getenv用 */
#include <stdio.h>  /* snprintf用 */
#include <fcntl.h>  /* open用 */
#include <unistd.h> /* write, close用 */
#include "core/core.h"
#include "tuning.h"

/* ************************************************************************** */
int
	screenshot(t_game* game);
int
	save_bmp(t_game* game, int fd);
static void
	get_screenshot_path(char* buffer, size_t size);
static int
	write_bmp_header(int fd, int filesize, t_game* game);
static void
	set_int_in_char(unsigned char* start, int value);
static int
	write_bmp_data(int file, t_window* w, int pad);
static int
	get_color(t_window* w, int x, int y);

/* ************************************************************************** */
// スクリーンショットを撮ってファイルに保存し、ゲームを終了する。日時付きパスを作って開き、
// その fd を save_bmp に渡して最新フレームを書き出す。撮影は1回限りで、成功・失敗どちらの
// 経路でも fd を close してから exit_game / exit_error で終了する（書き込み先を日時付きパスに
// 一本化し、以前の二重オープンと fd リークを解消済み）
int
	screenshot(t_game* game)
{
	int		fd;
	char	filepath[256];

	get_screenshot_path(filepath, sizeof(filepath));
	fd = open(filepath, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0) {
		return (exit_error(game, "Error:\nFailed to create screenshot file in ~/bmp/.\n"));
	}
	render_frame(game);
	if (!save_bmp(game, fd)) {
		close(fd);
		exit_error(game, "Error:\nfailed to save screenshot.\n");
	}
	close(fd);
	return (exit_game(game, EXIT_SUCCESS));
}

/* ************************************************************************** */
// 呼び出し側 screenshot が開いた fd（日時付きパス）へ画面ピクセルを24bit BMP として書き出す。
// 1行を4バイト境界に揃えるパディング pad を計算し、ヘッダ54バイト＋(幅+pad)*高さ*3 を
// ファイルサイズに。ヘッダ→データの順に書き、いずれか失敗で 0 を返す。fd の open/close は
// 呼び出し側が一元管理するため、ここでは行わない（以前はここで別名を再オープンしていた）
int
	save_bmp(t_game* game, int fd)
{
	t_window*	w;
	int			filesize;
	int			pad;

	w = &game->window;
	pad = (4 - ((int)w->size.x * 3) % 4) % 4;
	filesize = 54 + (3 * ((int)w->size.x + pad) * (int)w->size.y);
	if (!write_bmp_header(fd, filesize, game)) {
		return (0);
	}
	if (!write_bmp_data(fd, w, pad)) {
		return (0);
	}
	return (1);
}

/* ************************************************************************** */
// 現在時刻から "<home>/bmp/screenshot_YYYY_MMDD_HHMM.bmp" 形式のパスを buffer に生成する。
// home は暫定で "."（カレントディレクトリ）固定。snprintf で size を超えないよう切り詰める
static void
	get_screenshot_path(char* buffer, size_t size)
{
	time_t		t;
	struct tm*	tm_info;
	char*		home;

	t = time(NULL);
	tm_info = localtime(&t);
	home = ".";
	snprintf(buffer, size, "%s/bmp/screenshot_%04d_%02d%02d_%02d%02d.bmp",
		home,
		tm_info->tm_year + 1900,
		tm_info->tm_mon + 1,
		tm_info->tm_mday,
		tm_info->tm_hour,
		tm_info->tm_min);
}

/* ************************************************************************** */
// BMP の54バイトヘッダ（14バイトのファイルヘッダ＋40バイトの情報ヘッダ）を組み立てて書く。
// 先頭を 'B''M'、オフセット2にファイルサイズ、10にデータ開始位置(54)、14に情報ヘッダサイズ
// (40)、18/22に幅・高さ、27に面数(1)、28にビット深度(24)を設定。write 失敗時は 0 を返す
static int
	write_bmp_header(int fd, int filesize, t_game* game)
{
	int				i;
	int				tmp;
	unsigned char	bmpfileheader[54];

	i = 0;
	while (i < 54) {
		bmpfileheader[i++] = (unsigned char)(0);
	}
	bmpfileheader[0] = (unsigned char)('B');
	bmpfileheader[1] = (unsigned char)('M');
	set_int_in_char(bmpfileheader + 2, filesize);
	bmpfileheader[10] = (unsigned char)(54);
	bmpfileheader[14] = (unsigned char)(40);
	tmp = game->window.size.x;
	set_int_in_char(bmpfileheader + 18, tmp);
	tmp = game->window.size.y;
	set_int_in_char(bmpfileheader + 22, tmp);
	bmpfileheader[27] = (unsigned char)(1);
	bmpfileheader[28] = (unsigned char)(24);
	return (!(write(fd, bmpfileheader, 54) < 0));
}

/* ************************************************************************** */
// 32bit 整数を start から4バイトにリトルエンディアン（下位バイトが先頭）で書き込む。
// BMP ヘッダの数値フィールドはリトルエンディアン規定のため、この順で格納する
static void
	set_int_in_char(unsigned char* start, int value)
{
	start[0] = (unsigned char)(value);
	start[1] = (unsigned char)(value >> 8);
	start[2] = (unsigned char)(value >> 16);
	start[3] = (unsigned char)(value >> 24);
}

/* ************************************************************************** */
// 画面ピクセルを BMP のデータ領域として1行ずつ書き出す。BMP は1ピクセル3バイト(BGR)で、
// 行末を4バイト境界に揃えるため pad バイトのゼロを書き足す。write 失敗時は 0 を返す
static int
	write_bmp_data(int file, t_window* w, int pad)
{
	const unsigned char	zero[3] = {0, 0, 0};
	int					i;
	int					j;
	int					color;

	i = 0;
	while (i < (int)w->size.y) {
		j = 0;
		while (j < (int)w->size.x) {
			color = get_color(w, j, i);
			if (write(file, &color, 3) < 0) {
				return (0);
			}
			if (pad > 0 && write(file, &zero, pad) < 0) {
				return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
}

/* ************************************************************************** */
// 画面バッファから (x, y) のピクセル色を取得する。BMP は最下行から上へ並ぶ「ボトムアップ」
// 形式のため、行を (高さ-1-y) に反転して読む。返すのは下位24bit(0xRRGGBB)
static int
	get_color(t_window* w, int x, int y)
{
	int	rgb;
	int	color;

	color = *(int*)(w->screen.ptr + (BYTES_PER_PIXEL * (int)w->size.x * ((int)w->size.y - 1 - y)) + (BYTES_PER_PIXEL * x));
	rgb = color & 0xFFFFFF;
	return (rgb);
}
