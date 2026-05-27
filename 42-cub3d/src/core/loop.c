#include "cub3d.h"

int	main_loop(t_game *game)
{
	static int	update = 1;
	static int	last_opt = 0x00000111;

	if (game->move.x || game->move.y)
		update = move_camera(game, (game->move.x) ? 0 : 1);
	if (game->x_move.x || game->x_move.y)
		update = move_perp_camera(game, (game->x_move.x) ? 0 : 1);
	if (game->rotate.x || game->rotate.y)
		update = rotate_camera(game, (game->rotate.x) ? 0 : 1);
	if (last_opt != game->options)
	{
		update = 1;
		last_opt = game->options;
	}
	if (update)
	{
		if (game->to_collect > 0)
			check_quest(game);
		MAP(game->camera.pos, game->config) = 'A';
		render_frame(game);
	}
	update = 0;
	return (0);
}
