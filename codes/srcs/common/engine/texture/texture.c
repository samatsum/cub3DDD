#include <stdio.h>

#include "engine/render/render.h"
#include "engine/texture/texture.h"
#include "config/config.h"
#include "../minilibx-linux/mlx.h"

/* ************************************************************************** */
int
	load_textures(t_window* window, t_tex* tex, t_config* config);
void
	clear_textures(t_window* window, t_tex* tex);
static int
	load_tex(t_window* window, t_tex* tex, char* path);
static void
	compute_tex_bounds(t_tex* tex);
static int
	line_is_empty(t_tex* tex, int line);
static int
	column_is_empty(t_tex* tex, int column);

/* ************************************************************************** */
// config の全テクスチャパスを順に読み込み、読めたものは compute_tex_bounds で透明縁を
// 除いた有効描画領域(start/end)を計算する。1枚でも読み込み失敗（パスはあるが画像が無効）
// なら、どのファイルかを表示して 0 を返す。パスが NULL の枠は load_tex 内でスキップされ、
// tex 画像が生成されないため領域計算も行わない
int
	load_textures(t_window* window, t_tex* tex, t_config* config)
{
	int	i;

	i = 0;
	while (i < TEXTURES) {
		if (!load_tex(window, &tex[i], config->tex_path[i])) {
			printf("Failed to load texture: %s\n", config->tex_path[i]);
			return (0);
		}
		if (tex[i].tex) {
			compute_tex_bounds(&tex[i]);
		}
		i++;
	}
	return (1);
}

/* ************************************************************************** */
// 読み込んだ全テクスチャの mlx 画像を破棄し、tex/ptr を NULL に戻して二重解放を防ぐ
void
	clear_textures(t_window* window, t_tex* tex)
{
	int	i;

	i = 0;
	while (i < TEXTURES) {
		if (tex[i].tex) {
			mlx_destroy_image(window->ptr, tex[i].tex);
		}
		tex[i].tex = NULL;
		tex[i].ptr = NULL;
		i++;
	}
}

/* ************************************************************************** */
// パスから XPM 画像を読み込み、画像ハンドルとピクセル先頭アドレス(ptr)を tex に格納する。
// path が NULL（その種別が .cub 未指定）なら何もせず成功扱い(1)。パスはあるが読み込みに
// 失敗した場合のみ 0 を返す
static int
	load_tex(t_window* window, t_tex* tex, char* path)
{
	if (path) {
		tex->path = path;
		tex->tex = mlx_xpm_file_to_image(window->ptr, path, &tex->width, &tex->height);
		if (tex->tex) {
			tex->ptr = mlx_get_data_addr(tex->tex, &tex->bpp, &tex->size_line, &tex->endian);
		} else {
			return (0);
		}
	}
	return (1);
}

/* ************************************************************************** */
// テクスチャの透明な外周を除いた有効描画領域(start/end)を算出する。x方向は列番号
// (0..width)、y方向は行番号(0..height)で走査し、draw時の左右/上下トリミングに使う
static void
	compute_tex_bounds(t_tex* tex)
{
	int	j;

	j = 0;
	while (j < tex->width && column_is_empty(tex, j)) {
		j++;
	}
	tex->start.x = j;
	while (j < tex->width && !column_is_empty(tex, j)) {
		j++;
	}
	tex->end.x = j;
	j = 0;
	while (j < tex->height && line_is_empty(tex, j)) {
		j++;
	}
	tex->start.y = j;
	while (j < tex->height && !line_is_empty(tex, j)) {
		j++;
	}
	tex->end.y = j;
}

/* ************************************************************************** */
// テクスチャの line 行目が全幅にわたり透明(色 0x0)かを判定する。1ピクセルでも不透明なら 0。
// 上下の透明縁をトリミングする start.y/end.y の算出に使う
static int
	line_is_empty(t_tex* tex, int line)
{
	int		i;
	t_pos	pos;

	pos.y = line;
	i = 0;
	while (i < tex->width) {
		pos.x = i;
		if (get_tex_color(tex, &pos) != 0x0) {
			return (0);
		}
		i++;
	}
	return (1);
}

/* ************************************************************************** */
// テクスチャの column 列目が全高にわたり透明(色 0x0)かを判定する。1ピクセルでも不透明なら 0。
// 左右の透明縁をトリミングする start.x/end.x の算出に使う
static int
	column_is_empty(t_tex* tex, int column)
{
	int		i;
	t_pos	pos;

	pos.x = column;
	i = 0;
	while (i < tex->height) {
		pos.y = i;
		if (get_tex_color(tex, &pos) != 0x0) {
			return (0);
		}
		i++;
	}
	return (1);
}
