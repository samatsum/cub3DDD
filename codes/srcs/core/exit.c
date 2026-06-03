/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 19:25:55 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/04 02:49:47 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/core.h"
#include "gnl/get_next_line.h"     /* 追加: GNLの強制クリーンアップ呼び出しのため */
#include "../minilibx-linux/mlx.h" /* mlx_destroy_display 等のため */
#include <stdlib.h>                /* free, exit 関数を使用するため */
#include <unistd.h>                /* write 関数, STDOUT_FILENO用 */

/* ************************************************************************** */
int
	exit_error(t_game* game, char const* str);
int
	exit_game(t_game* game, int code);
int
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
// 使用したメモリやリソースを解放し、ゲームを終了させる
int
	exit_game(t_game* game, int code)
{
	/* 修正2: プログラム終了時に、GNL(get_next_line)の静的バッファに
	   読みかけのデータが残っていた場合(Still Reachable)のメモリリークを完全に防ぐ */
	get_next_line(-1, NULL);

	clear_config(&game->config);
	clear_window(&game->window);
	clear_textures(&game->window, game->tex);
	clear_sprites(&game->sprites);
	if (game->window.ptr) {
		mlx_destroy_display(game->window.ptr);
		free(game->window.ptr);
	}
	exit(code);
	return (code);
}

/* ************************************************************************** */
// ウィンドウとイメージのリソースを解放する
int
	clear_window(t_window* window)
{
	if (window->screen.img) {
		mlx_destroy_image(window->ptr, window->screen.img);
	}
	if (window->ptr && window->win) {
		mlx_destroy_window(window->ptr, window->win);
	}
	return (0);
}
