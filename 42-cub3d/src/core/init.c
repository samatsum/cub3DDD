#include "cub3d.h"
#include "math_engine.h"

int
	init_image(t_window *window, t_image *img)
{
	if (!(img->img =
		mlx_new_image(window->ptr, window->size.x, window->size.y)))
		return (0);
	img->ptr = mlx_get_data_addr(
		img->img, &img->bpp, &img->size_line, &img->endian);
	return (1);
}

int
	init_window(t_window *window, t_config *config)
{
	set_pos(&window->size, config->requested_width, config->requested_height);
	if (window->size.x > 1920)
		window->size.x = 1920;
	if (window->size.y > 1080)
		window->size.y = 1080;
	if (window->size.x < 848)
		window->size.x = 848;
	if (window->size.y < 480)
		window->size.y = 480;
	window->ptr = NULL;
	window->win = NULL;
	window->ratio = window->size.x / window->size.y;
	window->screen.img = NULL;
	if (window->ratio < BEST_RATIO)
		config->fov = config->fov / ((BEST_RATIO / config->fov) / 2.5);
	else if (window->ratio > BEST_RATIO)
		config->fov = config->fov * ((config->fov / BEST_RATIO) * 2.5);
	if (!(window->ptr = mlx_init())
		|| !(window->win = mlx_new_window(
			window->ptr, window->size.x, window->size.y, "cub3d")))
		return (0);
	set_pos(&window->half, window->size.x / 2, window->size.y / 2);
	if (!init_image(window, &window->screen))
		return (0);
	return (1);
}

void
	init_game(t_game *game, int save_opt)
{
	int	i;

	init_config(&game->config);
	set_pos(&game->move, 0, 0);
	set_pos(&game->x_move, 0, 0);
	set_pos(&game->rotate, 0, 0);
	game->collected = 0;
	game->options = 0x11111110;
	game->sprites = NULL;
	if (save_opt)
		game->options = game->options | FLAG_SAVE;
	i = 0;
	while (i < TEXTURES)
		game->tex[i++].tex = NULL;
}

static int
	find_sprites(t_game *game)
{
	int		i;
	int		j;
	t_pos	pos;
	char	c;
	t_tex	*tex;

	game->sprites = NULL;
	i = 0;
	while (i < game->config.rows)
	{
		j = 0;
		while (j < game->config.columns)
		{
			set_pos(&pos, j + .5, i + .5);
			c = MAP(pos, game->config);
			tex = &game->tex[TEX_SPRITE + (c - '0' - 2)];
			if (c >= '2' && c <= '4' && tex->tex
				&& !add_front_sprite(&game->sprites, 0., &pos, tex))
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}


int
	finish_init(t_game *game)
{
	if (!init_window(&game->window, &game->config))
	{
		return (exit_error(game,
			"Error:\nmlx failed to create window or image.\n"));
	}
	find_start_pos(&game->config, &game->camera);
	find_start_angle(&game->config, &game->camera);
	if (!load_textures(game))
		return (exit_error(game, "Error:\nfailed to load texture(s).\n"));
	if (!find_sprites(game))
		return (exit_error(game, "Error:\nfailed to malloc sprites.\n"));
	count_items(game);
	make_tables(game);
	return (1);
}