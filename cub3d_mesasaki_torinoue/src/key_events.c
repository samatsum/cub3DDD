/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_events.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: torinoue <torinoue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 22:30:02 by torinoue          #+#    #+#             */
/*   Updated: 2025/11/07 00:15:42 by torinoue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

// static void			debug_print_player_state(const t_player *player);
static const char	*get_wall_direction_string(const double move_angle);

void	move_player(t_player *player, const t_map_context *map_ctx, \
	const double move_angle)
{
	t_map_coord	new_coord;

	new_coord.x = player->pos.x + cos(move_angle) * DEFAULT_MOVE_VALUE;
	new_coord.y = player->pos.y + sin(move_angle) * DEFAULT_MOVE_VALUE;
	if (is_on_wall(map_ctx, &new_coord))
	{
		player->is_colliding = true;
		printf("COLLISION : coord(%09.5f, %09.5f) grid[%03d, %03d] \
angle=%06.2f° wall=%s\n", player->pos.x, player->pos.y,
			(int)(player->pos.x / map_ctx->tile_size), (int)(player->pos.y \
		/ map_ctx->tile_size), normalize_angle(move_angle) * 180.0 / PI, \
			get_wall_direction_string(move_angle));
		return ;
	}
	player->pos.x = new_coord.x;
	player->pos.y = new_coord.y;
	player->is_colliding = false;
	printf("Move OK   : coord(%09.5f, %09.5f) grid[%03d, %03d] \
angle=%06.2f°\n",
		player->pos.x, player->pos.y, (int)(player->pos.x / \
	map_ctx->tile_size), (int)(player->pos.y / map_ctx->tile_size),
		normalize_angle(move_angle) * 180.0 / PI);
}

// int	key_press_event(int key, t_data *data)
// {
// 	t_key_event_context	key_ctx;

// 	if (!data)
// 		return (0);
// 	if (key == KEY_ESC)
// 	{
// 		clean_exit(data, EXIT_SUCCESS);
// 		return (0);
// 	}
// 	key_ctx.player = &data->game.player;
// 	key_ctx.map_ctx = &data->map_ctx;
// 	handle_key_event(key, &key_ctx);
// 	debug_print_player_state(&data->game.player);
// 	return (0);
// }

// static void	debug_print_player_state(const t_player *player)
// {
// 	printf("pos x:%f pos y:%f angle:%f°\n",
// 		player->pos.x,
// 		player->pos.y,
// 		normalize_angle(player->angle) * 180.0 / PI);
// }

/**
 * @brief Get direction string based on movement angle
 * @param move_angle Movement angle in radians
 * @return Direction string (N/S/E/W)
 */
static const char	*get_wall_direction_string(const double move_angle)
{
	double	normalized_angle;

	normalized_angle = normalize_angle(move_angle);
	if (normalized_angle >= 7 * PI / 4 || normalized_angle < PI / 4)
		return ("E");
	else if (normalized_angle >= PI / 4 && normalized_angle < 3 * PI / 4)
		return ("S");
	else if (normalized_angle >= 3 * PI / 4 && normalized_angle < 5 * PI / 4)
		return ("W");
	else
		return ("N");
}
/*
 valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all

 valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all


 */