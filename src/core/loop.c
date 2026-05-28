#include "cub3d.h"
#include "math_engine.h"

int
	main_loop(t_game *game)
{
//補足：現在の課題（静的変数の依存解消）へのアプローチ
// 現在のコードでは static int last_opt によって前回の状態を保持している。これは資料の「⚠️ 現在の課題」にある「静的変数への依存（状態管理のアンチパターン）」そのものである。複数回の状態遷移や、ゲームの再初期化が行われた際に、この静的変数が初期化されずに古い状態を引きずり、予期せぬ挙動を生むリスクがある。

// これを根本的に解決（Phase 2の完了）するためには、将来的に t_game 構造体（または t_window などの適切な構造体）のメンバに変数を移行することが望ましい。

// 移行する場合の設計案：

// t_game 構造体に int last_options; を追加する。

// init_game 時に game->last_options = 0x00000111; で初期化する。

// main_loop 内では if (game->options != game->last_options) として比較・更新を行う。
	static int	last_opt = 0x00000111;
	int			update;

	update = 0;
	if (game->move.x || game->move.y)
		update = move_camera(&game->camera, &game->config, (game->move.x) ? 0 : 1);
	if (game->x_move.x || game->x_move.y)
		update = move_perp_camera(&game->camera, &game->config, (game->x_move.x) ? 0 : 1);
	if (game->rotate.x || game->rotate.y)
		update = rotate_camera(&game->camera, game->cos, game->sin, (game->rotate.x) ? 0 : 1);
	if (game->options != last_opt)
	{
		update = 1;
		last_opt = game->options;
	}
	if (update || (FLAG_UI & game->options))
		render_frame(game);
	return (1);
}
