/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_events_hooks.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: torinoue <torinoue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 15:00:00 by torinoue          #+#    #+#             */
/*   Updated: 2025/11/03 15:00:00 by torinoue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

/**
 * @brief Unified hook handler for all keys (ESC + normal keys)
 * @param key Pressed key code
 * @param param Pointer to t_data (MLX passes this as void *)
 * @return Always 0 (MLX specification)
 * 
 * MLX limitation: Only one hook per event type is allowed.
 * This function handles both ESC (requires data)
 *  and normal keys (minimal context).
 * ESC key is handled early with full data access for clean_exit.
 * Normal keys use minimal context (player + map_ctx only).
 */
int	key_press_unified(int key, void *param)
{
	t_data				*data;
	t_key_event_context	key_ctx;

	data = (t_data *)param;
	if (!data)
		return (0);
	if (key == KEY_ESC)
	{
		clean_exit(data, EXIT_SUCCESS);
		return (0);
	}
	key_ctx.player = &data->game.player;
	key_ctx.map_ctx = &data->map_ctx;
	handle_key_event(key, &key_ctx);
	return (0);
}

/**
 * @brief Replace existing key_press_event function with callback version
 * @param key Pressed key code
 * @param data Game data
 * @return Always 0 (MLX specification)
 * 
 * Maintains existing interface while changing internal implementation
 * to callback version
 */
// int	key_press_event_callback(int key, t_data *data)
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
// 	return (0);
// }
