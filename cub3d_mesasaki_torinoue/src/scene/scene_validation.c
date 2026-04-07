/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_validation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: torinoue <torinoue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 21:03:45 by torinoue          #+#    #+#             */
/*   Updated: 2025/11/06 20:34:25 by torinoue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	validate_scene_settings(t_data *data)
{
	if (is_invalid_setting_order(data->scene_filepath, data))
		display_setting_error(INVALID_SETTING_ORDER, data);
	if (is_invalid_texture_setting(data->scene_filepath, data))
		display_setting_error(INVALID_TEXTURE_SETTING, data);
	if (is_invalid_color_setting(data->scene_filepath, data))
		display_setting_error(INVALID_COLOR_SETTING, data);
	if (is_invalid_number_map_elem(data->scene_filepath, data))
		display_setting_error(INVALID_NUMBER_MAP_ELEM, data);
}

void	validate_map_format(t_data *data)
{
	if (is_invalid_map_elem(data->scene_filepath, &data->map_ctx, data))
		display_setting_error(INVALID_MAP_ELEM, data);
	if (is_invalid_map_unclosed(&data->map_ctx))
		display_setting_error(INVALID_MAP_UNCLOSED, data);
}
