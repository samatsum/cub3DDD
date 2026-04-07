/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_player_count_validation.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: torinoue <torinoue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 21:03:45 by torinoue          #+#    #+#             */
/*   Updated: 2025/11/03 21:21:56 by torinoue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/cub3d.h"

static bool	count_player_in_line_callback(const char *line, \
	void *player_count_ctx);

// Returns true if the number of player elements is invalid (must be exactly 1)
bool	is_invalid_number_map_elem(const char *filepath, t_data *data)
{
	int		player_count;
	bool	res;

	player_count = 0;
	res = check_lines_with_callback(filepath, \
		count_player_in_line_callback, &player_count, data);
	if (player_count != 1)
		res = true;
	return (res);
}

/**
 * @brief Count player characters in each map line (callback version)
 * 
 * This function counts NSWE characters in map lines
 *  (lines starting with '1' or ' ').
 * It's used as a callback for check_lines_with_callback().
 * The count is accumulated in the context variable.
 * 
 * @param line Current line being processed
 * @param player_count_ctx Pointer to int that stores the player count
 * @return false (always) to continue processing all lines
 */
static bool	count_player_in_line_callback(const char *line, \
	void *player_count_ctx)
{
	int	*player_count;
	int	i;

	player_count = (int *)player_count_ctx;
	i = -1;
	if (line[0] == '1' || line[0] == ' ')
	{
		while (line[++i])
		{
			if (ft_strchr("NSWE", line[i]))
				++*player_count;
		}
	}
	return (false);
}
