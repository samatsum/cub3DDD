/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* main.c                                             :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/11/29 12:45:06 by samatsum          #+#    #+#             */
/* Updated: 2026/05/28 21:10:25 by samatsum         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdbool.h>
#include "cub3d.h"
#include "core/core.h"

int
	main(int argc, char **argv);
static bool
	validate_check(int argc, char **argv, t_game *game);
static bool
	setup_inits(t_game *game);
static void
	setup_hooks(t_game *game);

int
	main(int argc, char **argv)
{
	t_game	game;

	// 検証とマップ解析
	if (!validate_check(argc, argv, &game))
		return (EXIT_FAILURE);

	// 初期化処理
	if (!setup_inits(&game))
		return (EXIT_FAILURE);

	// イベントフックの設定とメインループ
	setup_hooks(&game);
	mlx_loop(game.window.ptr);
	return (EXIT_SUCCESS);
}

// 引数とマップファイルの検証を行う
static bool
	validate_check(int argc, char **argv, t_game *game)
{
	if (argc < 2)
	{
		exit_error(game, "Error:\nno map specified.\n");
		return (false);
	}
	init_config(&game->config);
	if (!parse_config(&game->config, argv[1]))
	{
		exit_error(game, "Error:\ninvalid map.\n");
		return (false);
	}
	return (true);
}

// システムの初期化処理を完了させる
static bool
	setup_inits(t_game *game)
{
	init_game(game);
	if (!finish_init(game))
		return (false);
	return (true);
}

static void
	setup_hooks(t_game *game)
{
	mlx_hook(game->window.win, X_EVENT_KEY_PRESS, MASK_KEY_PRESS, &key_press, game);
	mlx_hook(game->window.win, X_EVENT_KEY_RELEASE, MASK_KEY_RELEASE, &key_release, game);
	mlx_hook(game->window.win, X_EVENT_EXIT, MASK_CLOSE, &exit_hook, game);
	mlx_hook(game->window.win, EVENT_EXPOSE, MASK_EXPOSE, &expose_hook, game);
	mlx_loop_hook(game->window.ptr, &main_loop, game);
}
