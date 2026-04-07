/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_value.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: torinoue <torinoue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 02:04:33 by toruinoue         #+#    #+#             */
/*   Updated: 2025/11/07 18:58:07 by torinoue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d.h"

static bool	check_comma_count(const char *str);
static bool	check_comma_position(const char *str);
static bool	check_rgb_range(char *const *rgb_array);
static bool	check_digit_string(const char *str);

bool	is_invalid_color_value(const char *str)
{
	char	**tmp_arr;
	bool	res;

	if (!str || check_comma_count(str) || check_comma_position(str))
		return (true);
	tmp_arr = ft_split(str, ',');
	if (!tmp_arr)
		return (true);
	res = (arrlen(tmp_arr) != 3) || \
		(check_digit_string(tmp_arr[0]) || check_digit_string(tmp_arr[1]) || \
		check_digit_string(tmp_arr[2])) || check_rgb_range(tmp_arr);
	free_str_array(&tmp_arr);
	return (res);
}

static bool	check_comma_count(const char *str)
{
	int		comma_count;
	int		i;

	comma_count = 0;
	i = -1;
	while (str[++i])
		(str[i] == ',') && (comma_count++);
	return (comma_count != 2);
}

static bool	check_comma_position(const char *str)
{
	char	*tmp;

	tmp = ft_strchr(str, ',');
	return (str[0] == ',' || str[ft_strlen(str) - 1] == ',' || !tmp || \
		!ft_strchr(tmp + 1, ','));
}

static bool	check_rgb_range(char *const *rgb_array)
{
	return (ft_atoi(rgb_array[0]) > 255 || ft_atoi(rgb_array[0]) < 0 || \
		ft_atoi(rgb_array[1]) > 255 || ft_atoi(rgb_array[1]) < 0 || \
		ft_atoi(rgb_array[2]) > 255 || ft_atoi(rgb_array[2]) < 0);
}

/*
** Checks if the string contains at least one digit and only allows
** digits, spaces, and newlines. Whitespace-only strings return true.
** This is specifically for validating RGB color values.
**
** @param str: The string to validate
** @return: true if invalid, false if valid
*/
static bool	check_digit_string(const char *str)
{
	int		i;
	bool	has_digit;

	i = -1;
	has_digit = false;
	while (str[++i])
	{
		if (!ft_isdigit(str[i]) && str[i] != '\n' && str[i] != ' ')
			return (true);
		if (ft_isdigit(str[i]))
			has_digit = true;
	}
	return (!has_digit);
}
