/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 19:25:55 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/06 14:34:52 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/core.h"
#include "gnl/get_next_line.h"
#include "../minilibx-linux/mlx.h"
#include <stdlib.h>
#include <unistd.h>

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
	int i;

	get_next_line(-1, NULL);

	if (game)
	{
		clear_config(&game->config);
		clear_window(&game->window);
		clear_textures(&game->window, game->tex);
		clear_sprites(&game->sprites);
		
		/* 3つの武器メモリを解放 */
		i = 0;
		while (i < 3) {
			if (game->weapon_tex[i].tex && game->window.ptr)
				mlx_destroy_image(game->window.ptr, game->weapon_tex[i].tex);
			if (game->weapon_tex[i].path)
				free(game->weapon_tex[i].path);
			i++;
		}

		if (game->window.ptr) {
			mlx_destroy_display(game->window.ptr);
			free(game->window.ptr);
		}
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