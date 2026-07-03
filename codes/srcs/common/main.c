#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include "core/core.h"
#include "core/mode_ops.h"
#include "engine/input/input.h"
#include "engine/input/keymap.h"
#include "../minilibx-linux/mlx.h"

#define CUB_ARG_MAP 1
#define CUB_ARG_MODE 2
#define CUB_ARG_COUNT 3
#define FPS_MAP_DIR "maps/fps_map/"
#define RSP_MAP_DIR "maps/rsp_map/"

/* ************************************************************************** */
int
	main(int argc, char** argv);
static bool
	validate_check(int argc, char** argv, t_game* game);
static bool
	validate_map_path(char const* path, int mode, t_game* game);
static bool
	validate_map_file(char const* path, t_game* game);
static bool
	path_contains(char const* path, char const* needle);
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
// 引数とマップファイルの検証を行う。argv[CUB_ARG_MODE] で FPS/RSP モードを明示的に選ぶ
static bool
	validate_check(int argc, char** argv, t_game* game)
{
	if (argc <= CUB_ARG_MAP) {
		exit_error(game, "Error:\n no map specified.\n");
		return (false);
	}
	if (argc < CUB_ARG_COUNT) {
		exit_error(game, "Error:\n mode argument required. use FPS or RSP.\n");
		return (false);
	}
	if (argc > CUB_ARG_COUNT) {
		exit_error(game, "Error:\n too many arguments.\n");
		return (false);
	}
	if (ft_strcmp(argv[CUB_ARG_MODE], "RSP") == 0) {
		game->mode = MODE_RSP;
		game->mode_ops = rsp_mode_ops();
	} else if (ft_strcmp(argv[CUB_ARG_MODE], "FPS") == 0) {
		game->mode = MODE_FPS;
		game->mode_ops = fps_mode_ops();
	} else {
		exit_error(game, "Error:\n invalid mode. use FPS or RSP.\n");
		return (false);
	}
	if (!validate_map_path(argv[CUB_ARG_MAP], game->mode, game)) {
		return (false);
	}
	if (!validate_map_file(argv[CUB_ARG_MAP], game)) {
		return (false);
	}
	init_config(&game->config);
	if (!parse_config(&game->config, argv[CUB_ARG_MAP])) {
		exit_error(game, "Error:\n invalid map content. check syntax, walls, characters, and spawns.\n");
		return (false);
	}
	return (true);
}


/* ************************************************************************** */
// モードとマップ格納ディレクトリの対応を検証する
static bool
	validate_map_path(char const* path, int mode, t_game* game)
{
	if (mode == MODE_FPS && !path_contains(path, FPS_MAP_DIR)) {
		exit_error(game, "Error:\n FPS mode requires a map in maps/fps_map/.\n");
		return (false);
	}
	if (mode == MODE_RSP && !path_contains(path, RSP_MAP_DIR)) {
		exit_error(game, "Error:\n RSP mode requires a map in maps/rsp_map/.\n");
		return (false);
	}
	return (true);
}


/* ************************************************************************** */
// マップファイルそのものを検証する（拡張子と open 可否）
static bool
	validate_map_file(char const* path, t_game* game)
{
	int	fd;

	if (!ft_endswith(path, ".cub")) {
		exit_error(game, "Error:\n map file must end with .cub.\n");
		return (false);
	}
	fd = open(path, O_RDONLY);
	if (fd < 0) {
		exit_error(game, "Error:\n failed to open map file.\n");
		return (false);
	}
	close(fd);
	return (true);
}

/* ************************************************************************** */
// path の中に needle が含まれるかを調べる小さな文字列検索
static bool
	path_contains(char const* path, char const* needle)
{
	int	i;
	int	j;

	i = 0;
	while (path[i]) {
		j = 0;
		while (path[i + j] && needle[j] && path[i + j] == needle[j]) {
			j++;
		}
		if (!needle[j]) {
			return (true);
		}
		i++;
	}
	return (false);
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
