/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation_elements.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: torinoue <torinoue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 21:03:45 by torinoue          #+#    #+#             */
/*   Updated: 2025/11/03 21:21:20 by torinoue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/cub3d_types.h"
#include "../../../../include/cub3d_functions.h"
#include "../../../../lib/libft/libft.h"

static bool	check_elem_callback(const char *line, void *elem_ctx);
static bool	is_invalid_splace_exists(const t_map_context *map_ctx);

bool	is_invalid_map_elem(const char *filepath, \
	const t_map_context *map_ctx, t_data *data)
{
	bool	res;

	res = false;
	res = check_lines_with_callback(filepath, \
		check_elem_callback, &res, data);
	res = res || is_invalid_splace_exists(map_ctx);
	return (res);
}

/**
 * @brief Check each line for invalid map elements (callback version)
 * 
 * This function checks if map lines contain only valid characters.
 * It's used as a callback for check_lines_with_callback().
 * 
 * @param line Current line being validated
 * @param elem_ctx Pointer to bool that stores the result
 * @return true if invalid element found, false otherwise
 */
static bool	check_elem_callback(const char *line, void *elem_ctx)
{
	bool	*res;
	int		i;

	res = (bool *)elem_ctx;
	i = -1;
	if (line[0] == '1' || line[0] == ' ')
	{
		while (line[++i] && !*res)
		{
			if (!ft_strchr("1 0NSWE\n", line[i]))
			{
				*res = true;
				return (true);
			}
		}
	}
	return (*res);
}

static bool	is_invalid_splace_exists(const t_map_context *map_ctx)
{
	int	y;
	int	x;

	y = 0;
	while (map_ctx->map[y])
	{
		x = 0;
		while (x < map_ctx->map_x_size[y])
		{
			if (map_ctx->map[y][x] == ' ')
			{
				if ((x > 0 && !ft_strchr("1 ", map_ctx->map[y][x - 1])) || \
				(y > 0 && x < map_ctx->map_x_size[y - 1] && !ft_strchr("1 ",
							map_ctx->map[y - 1][x])) || (map_ctx->map[y + 1]
						&& x < map_ctx->map_x_size[y + 1] && !ft_strchr("1 ",
							map_ctx->map[y + 1][x]))
						|| (x + 1 < map_ctx->map_x_size[y]
						&& !ft_strchr("1 ", map_ctx->map[y][x + 1])))
					return (true);
			}
			++x;
		}
		++y;
	}
	return (false);
}
