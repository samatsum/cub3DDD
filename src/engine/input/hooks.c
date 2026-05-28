#include "cub3d.h"


int	expose_hook(t_game *game)
{
	render_frame(game);
	return (0);
}

int	exit_hook(t_game *game)
{
	return (exit_game(game, EXIT_SUCCESS));
}

int	key_press(int keycode, t_game *game)
{
	if (keycode == KEY_W || keycode == KEY_FORWARD)
		game->move.x = 1;
	else if (keycode == KEY_S || keycode == KEY_BACKWARD)
		game->move.y = 1;
	if (keycode == KEY_A)
		game->x_move.x = 1;
	else if (keycode == KEY_D)
		game->x_move.y = 1;
	if (keycode == KEY_Q || keycode == KEY_LEFT)
		game->rotate.x = 1;
	else if (keycode == KEY_E || keycode == KEY_RIGHT)
		game->rotate.y = 1;
	return (0);
}

int	key_release(int keycode, t_game *game)
{
	if (keycode == KEY_W || keycode == KEY_FORWARD)
		game->move.x = 0;
	else if (keycode == KEY_S || keycode == KEY_BACKWARD)
		game->move.y = 0;
	else if (keycode == KEY_A)
		game->x_move.x = 0;
	else if (keycode == KEY_D)
		game->x_move.y = 0;
	else if (keycode == KEY_Q || keycode == KEY_LEFT)
		game->rotate.x = 0;
	else if (keycode == KEY_E || keycode == KEY_RIGHT)
		game->rotate.y = 0;
	else if (keycode == KEY_ESC)
		return (exit_game(game, EXIT_SUCCESS));
	else if (keycode == KEY_I)
		game->options = game->options ^ FLAG_UI;
	else if (keycode == KEY_L)
		game->options = game->options ^ FLAG_SHADOWS;
	else if (keycode == KEY_O)
		game->options = game->options ^ FLAG_CROSSHAIR;
	return (0);
}