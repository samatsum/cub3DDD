#include <stdbool.h>
#include <stdlib.h>
#include "core/core.h"
#include "core/mode_ops.h"
#include "engine/input/input.h"
#include "engine/input/keymap.h"
#include "../minilibx-linux/mlx.h"

/* ************************************************************************** */
int
	main(int argc, char** argv);
static bool
	validate_check(int argc, char** argv, t_game* game);
static bool
	setup_inits(t_game* game);
static void
	setup_hooks(t_game* game);

/* ************************************************************************** */
// メイン関数。プログラムのエントリポイントとして検証、初期化、メインループの実行を行う
int
	main(int argc, char** argv)
{
	t_game	game = {0};

	if (!validate_check(argc, argv, &game)) {
		return (EXIT_FAILURE);
	}
	if (!setup_inits(&game)) {
		return (EXIT_FAILURE);
	}
	setup_hooks(&game);
	mlx_loop(game.window.ptr);
	return (EXIT_SUCCESS);
}

/* ************************************************************************** */
// 引数とマップファイルの検証を行う。argv[2]=="RSP" でRSPモードを有効化する
static bool
	validate_check(int argc, char** argv, t_game* game)
{
	if (argc < 2) {
		exit_error(game, "Error:\n no map specified.\n");
		return (false);
	}
	init_config(&game->config);
	if (!parse_config(&game->config, argv[1])) {
		exit_error(game, "Error:\n invalid map.\n");
		return (false);
	}
	if (argc >= 3 && ft_strcmp(argv[2], "RSP") == 0) {
		game->mode = MODE_RSP;
		game->mode_ops = rsp_mode_ops();
	} else {
		game->mode = MODE_FPS;
		game->mode_ops = fps_mode_ops();
	}
	return (true);
}

/* ************************************************************************** */
// システムの初期化処理を完了させる
static bool
	setup_inits(t_game* game)
{
	init_game(game);
	if (!finish_init(game)) {
		return (false);
	}
	return (true);
}

/* ************************************************************************** */
// イベントフックの設定を行う
static void
	setup_hooks(t_game* game)
{
	mlx_hook(game->window.win, EVENT_KEY_PRESS, MASK_KEY_PRESS, &key_press, game);
	mlx_hook(game->window.win, EVENT_KEY_RELEASE, MASK_KEY_RELEASE, &key_release, game);
	mlx_hook(game->window.win, EVENT_EXIT, MASK_CLOSE, &exit_hook, game);
	mlx_hook(game->window.win, EVENT_EXPOSE, MASK_EXPOSE, &expose_hook, game);
	mlx_loop_hook(game->window.ptr, &main_loop, game);
}
