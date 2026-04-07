/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_order_validation.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: torinoue <torinoue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 00:45:00 by toruinoue         #+#    #+#             */
/*   Updated: 2025/11/03 21:21:26 by torinoue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/cub3d.h"

static bool	check_line_for_order_violation(const char *line, \
	t_header_parse_counters *counters);
static bool	check_duplicate_identifier(const char *line, \
	t_header_parse_counters *counters);
static bool	check_line_for_order_violation_callback(const char *line, \
	void *order_ctx);

bool	is_invalid_setting_order(const char *filepath, t_data *data)
{
	t_header_parse_counters	counters;

	counters.no_count = 0;
	counters.so_count = 0;
	counters.we_count = 0;
	counters.ea_count = 0;
	counters.f_count = 0;
	counters.c_count = 0;
	counters.is_map_start = false;
	return (check_lines_with_callback(filepath, \
		check_line_for_order_violation_callback, &counters, data));
}

static bool	check_line_for_order_violation_callback(const char *line, \
	void *order_ctx)
{
	return (check_line_for_order_violation(line, \
		(t_header_parse_counters *)order_ctx));
}

/**
 * @brief Check each line for counters order violations (Strict Implementation)
 * 
 * This function follows the fail-fast principle with strict interpretation:
 * - Map content must be the absolute last content in the file
 * - No empty lines or any other content allowed after map starts
 * - Immediately returns true when any non-map line appears after map start
 * 
 * Policy: "the map content which always has to be the last"
 * Interpretation: Map must end at EOF, no trailing content of any kind
 * 
 * @param line   Current line being validated
 * @return true if a violation is found, false otherwise
 */
static bool	check_line_for_order_violation(const char *line, \
	t_header_parse_counters *counters)
{
	if (check_duplicate_identifier(line, counters))
		return (true);
	if (line[0] == '1' || line[0] == ' ')
	{
		counters->is_map_start = true;
		if (counters->no_count != 1 || counters->so_count != 1
			|| counters->we_count != 1 || counters->ea_count != 1
			|| counters->f_count != 1 || counters->c_count != 1)
			return (true);
	}
	else if (counters->is_map_start)
		return (true);
	return (false);
}

static bool	check_duplicate_identifier(const char *line, \
	t_header_parse_counters *counters)
{
	return ((ft_strncmp(line, "NO ", 3) == 0
			&& ++counters->no_count > 1)
		|| (ft_strncmp(line, "SO ", 3) == 0
			&& ++counters->so_count > 1)
		|| (ft_strncmp(line, "WE ", 3) == 0
			&& ++counters->we_count > 1)
		|| (ft_strncmp(line, "EA ", 3) == 0
			&& ++counters->ea_count > 1)
		|| (ft_strncmp(line, "F ", 2) == 0
			&& ++counters->f_count > 1)
		|| (ft_strncmp(line, "C ", 2) == 0
			&& ++counters->c_count > 1));
}
