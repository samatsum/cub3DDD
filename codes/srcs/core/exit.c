#include "cub3d.h"
#include "core/core.h"

int	exit_error(t_game *game, char const *str)
{
	int	unused;

	if (str)
		unused = write(STDOUT_FILENO, str, ft_strlen(str));
	(void)unused;
	exit_game(game, EXIT_FAILURE);
	return (EXIT_FAILURE);
}

int
	exit_game(t_game *game, int code)
{
	clear_config(&game->config);
	clear_window(&game->window);
	clear_textures(&game->window, game->tex);
	clear_sprites(&game->sprites);
	exit(code);
	return (code);
}

int
	clear_window(t_window *window)
{
	if (window->screen.img)
		mlx_destroy_image(window->ptr, window->screen.img);
	if (window->ptr && window->win)
		mlx_destroy_window(window->ptr, window->win);
	return (0);
}