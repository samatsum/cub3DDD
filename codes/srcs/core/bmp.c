/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 18:13:15 by samatsum          #+#    #+#             */
/*   Updated: 2026/05/29 03:04:14 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <time.h>   /* time, localtime用 */
#include <stdlib.h> /* getenv用 */
#include <stdio.h>  /* snprintf用 */
#include <fcntl.h>  /* open用 */
#include <unistd.h> /* write, close用 */

static void
	set_int_in_char(unsigned char *start, int value)
{
	start[0] = (unsigned char)(value);
	start[1] = (unsigned char)(value >> 8);
	start[2] = (unsigned char)(value >> 16);
	start[3] = (unsigned char)(value >> 24);
}

static int
	write_bmp_header(int fd, int filesize, t_game *game)
{
	int				i;
	int				tmp;
	unsigned char	bmpfileheader[54];

	i = 0;
	while (i < 54)
		bmpfileheader[i++] = (unsigned char)(0);
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

static int
	get_color(t_window *w, int x, int y)
{
	int	rgb;
	int	color;

	color = *(int*)(w->screen.ptr
			+ (4 * (int)w->size.x * ((int)w->size.y - 1 - y))
			+ (4 * x));
	rgb = (color & 0xFF0000) | (color & 0x00FF00) | (color & 0x0000FF);
	return (rgb);
}

static int
	write_bmp_data(int file, t_window *w, int pad)
{
	const unsigned char	zero[3] = {0, 0, 0};
	int					i;
	int					j;
	int					color;

	i = 0;
	while (i < (int)w->size.y)
	{
		j = 0;
		while (j < (int)w->size.x)
		{
			color = get_color(w, j, i);
			if (write(file, &color, 3) < 0)
				return (0);
			if (pad > 0 && write(file, &zero, pad) < 0)
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

int
	save_bmp(t_game *game)
{
	t_window	*w;
	int			filesize;
	int			file;
	int			pad;

	w = &game->window;
	pad = (4 - ((int)w->size.x * 3) % 4) % 4;
	filesize = 54 + (3 * ((int)w->size.x + pad) * (int)w->size.y);
	if ((file = open("screenshot.bmp", O_WRONLY | O_CREAT
									| O_TRUNC | O_APPEND, 0644)) < 0)
		return (0);
	if (!write_bmp_header(file, filesize, game))
		return (0);
	if (!write_bmp_data(file, w, pad))
		return (0);
	close(file);
	return (1);
}

/* 新規追加: ホームディレクトリと現在時刻からファイルパスを生成する関数 */
static void get_screenshot_path(char *buffer, size_t size)
{
    time_t      t;
    struct tm   *tm_info;
    char        *home;

    t = time(NULL);
    tm_info = localtime(&t);
    home = "."; /* 安全のため、相対パスでホームディレクトリを指定 (例: ../../) */

    /* フォーマット: ~/bmp/screenshot_YYYY_MMDD_HHMM.bmp */
    snprintf(buffer, size, "%s/bmp/screenshot_%04d_%02d%02d_%02d%02d.bmp",
             home,
             tm_info->tm_year + 1900,
             tm_info->tm_mon + 1,
             tm_info->tm_mday,
             tm_info->tm_hour,
             tm_info->tm_min);
}


int screenshot(t_game *game)
{
    int     fd;
    char    filepath[256]; /* パスを格納するバッファ */

    /* 修正: 固定の "screenshot.bmp" ではなく、動的パスを取得する */
    get_screenshot_path(filepath, sizeof(filepath));

    /* 取得したパスでファイルを作成・オープン */
    fd = open(filepath, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd < 0)
    {
        /* bmpディレクトリが存在しないなどの理由で失敗した場合の安全なエラー処理 */
        return (exit_error(game, "Error\nFailed to create screenshot file in ~/bmp/.\n"));
    }
	render_frame(game);
	if (!save_bmp(game))
		exit_error(game, "Error:\nfailed to save screenshot.");
	return (exit_game(game, EXIT_SUCCESS));
}
