/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 18:13:15 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/03 14:44:09 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <time.h>   /* time, localtime用 */
#include <stdlib.h> /* getenv用 */
#include <stdio.h>  /* snprintf用 */
#include <fcntl.h>  /* open用 */
#include <unistd.h> /* write, close用 */
#include "core/core.h"

/* ************************************************************************** */
int
	screenshot(t_game* game);
int
	save_bmp(t_game* game);
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
// 画面のスクリーンショットを撮影し、ファイルに保存してゲームを終了する
int
	screenshot(t_game* game)
{
	int		fd;
	char	filepath[256];

	get_screenshot_path(filepath, sizeof(filepath));
	fd = open(filepath, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0) {
		return (exit_error(game, "Error\nFailed to create screenshot file in ~/bmp/.\n"));
	}
	render_frame(game);
	if (!save_bmp(game)) {
		exit_error(game, "Error:\nfailed to save screenshot.");
	}
	return (exit_game(game, EXIT_SUCCESS));
}

/* ************************************************************************** */
// 画面のピクセルデータをBMP形式のファイルとして書き出す
int
	save_bmp(t_game* game)
{
	t_window*	w;
	int			filesize;
	int			file;
	int			pad;

	w = &game->window;
	pad = (4 - ((int)w->size.x * 3) % 4) % 4;
	filesize = 54 + (3 * ((int)w->size.x + pad) * (int)w->size.y);
	file = open("screenshot.bmp", O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, 0644);
	if (file < 0) {
		return (0);
	}
	if (!write_bmp_header(file, filesize, game)) {
		return (0);
	}
	if (!write_bmp_data(file, w, pad)) {
		return (0);
	}
	close(file);
	return (1);
}

/* ************************************************************************** */
// ホームディレクトリと現在時刻からスクリーンショットのファイルパスを生成する
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
// BMPファイルのヘッダー情報を書き込む
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
// 整数値を4バイトの文字配列にリトルエンディアンで設定する
static void
	set_int_in_char(unsigned char* start, int value)
{
	start[0] = (unsigned char)(value);
	start[1] = (unsigned char)(value >> 8);
	start[2] = (unsigned char)(value >> 16);
	start[3] = (unsigned char)(value >> 24);
}

/* ************************************************************************** */
// ウィンドウのピクセルデータをBMPのデータ領域として書き込む
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
// 指定された座標のピクセルの色（RGB値）を取得する
static int
	get_color(t_window* w, int x, int y)
{
	int	rgb;
	int	color;

	color = *(int*)(w->screen.ptr + (4 * (int)w->size.x * ((int)w->size.y - 1 - y)) + (4 * x));
	rgb = (color & 0xFF0000) | (color & 0x00FF00) | (color & 0x0000FF);
	return (rgb);
}
