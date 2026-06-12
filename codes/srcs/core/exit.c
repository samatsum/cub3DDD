#include <stdlib.h>
#include <unistd.h>
#include "../minilibx-linux/mlx.h"
#include "core/core.h"
#include "gnl/get_next_line.h"

/* ************************************************************************** */
int
	exit_error(t_game* game, char const* str);
int
	exit_game(t_game* game, int code);
static void
	clear_assets(t_game* game);
static int
	clear_window(t_window* window);

/* ************************************************************************** */
// エラーメッセージを出力し、ゲームを異常終了させる
int
	exit_error(t_game* game, char const* str)
{
	int	unused;

	if (str) {
		unused = write(STDOUT_FILENO, str, ft_strlen(str));
	}
	(void)unused;
	exit_game(game, EXIT_FAILURE);
	return (EXIT_FAILURE);
}

/* ************************************************************************** */
// 使用したメモリやリソースを完全に解放し、ゲームを終了させる
int
	exit_game(t_game* game, int code)
{
	get_next_line(-1, NULL);
	if (game) {
		clear_config(&game->config);
		clear_window(&game->window);
		clear_textures(&game->window, game->assets.tex);
		clear_sprites(&game->world.sprites);
		clear_enemies(&game->world.enemies);
		clear_assets(game);
		if (game->window.ptr) {
			mlx_destroy_display(game->window.ptr);
			free(game->window.ptr);
			game->window.ptr = NULL;
		}
	}
	exit(code);
	return (code);
}

/* ************************************************************************** */
// 武器(6種)と敵(8方向)のテクスチャ画像とパス文字列をすべて解放する
static void
	clear_assets(t_game* game)
{
	int	i;

	i = 0;
	while (i < WEAPON_TEX_COUNT) {
		if (game->assets.weapon_tex[i].tex && game->window.ptr) {
			mlx_destroy_image(game->window.ptr, game->assets.weapon_tex[i].tex);
			game->assets.weapon_tex[i].tex = NULL;
		}
		if (game->assets.weapon_tex[i].path) {
			free(game->assets.weapon_tex[i].path);
			game->assets.weapon_tex[i].path = NULL;
		}
		i++;
	}
	i = 0;
	while (i < ENEMY_TEX_COUNT) {
		if (game->assets.enemy_tex[i].tex && game->window.ptr) {
			mlx_destroy_image(game->window.ptr, game->assets.enemy_tex[i].tex);
			game->assets.enemy_tex[i].tex = NULL;
		}
		if (game->assets.enemy_tex[i].path) {
			free(game->assets.enemy_tex[i].path);
			game->assets.enemy_tex[i].path = NULL;
		}
		i++;
	}
}

/* ************************************************************************** */
// ウィンドウとイメージのリソースを解放する
static int
	clear_window(t_window* window)
{
	if (window->screen.img) {
		mlx_destroy_image(window->ptr, window->screen.img);
		window->screen.img = NULL;
	}
	if (window->ptr && window->win) {
		mlx_destroy_window(window->ptr, window->win);
		window->win = NULL;
	}
	return (0);
}
