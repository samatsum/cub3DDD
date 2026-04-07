/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toruinoue <toruinoue@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 21:03:45 by torinoue          #+#    #+#             */
/*   Updated: 2025/11/09 12:21:20 by toruinoue        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static const char	*get_setting_error_msg(t_setting_errors error);
static const char	*get_defined_error_msg(t_defined_errors error);

void	display_setting_error(t_setting_errors error, t_data *data)
{
	ft_printf_fd(STDERR_FILENO, "Error\n");
	ft_printf_fd(STDERR_FILENO, "%s: %s\n", data->scene_filepath,
		get_setting_error_msg(error));
	clean_exit(data, EXIT_FAILURE);
}

void	display_error(t_defined_errors error, t_data *data)
{
	ft_printf_fd(STDERR_FILENO, "Error\n");
	ft_printf_fd(STDERR_FILENO, "%s\n", get_defined_error_msg(error));
	clean_exit(data, EXIT_FAILURE);
}

static const char	*get_setting_error_msg(t_setting_errors error)
{
	const char	*error_msgs[SETTING_ERRORS] = {"The order of \
the settings is incorrect.",
		"The texture specification in the configuration file is incorrect.",
		"The color specification in the configuration file is incorrect.",
		"The configuration file contains an incorrect identifier.",
		"An invalid map element is being used.",
		"The number of map elements is invalid.",
		"Invalid unclosed map.",
		"The color value is invalid.",
		"Invalid map format."};

	return (error_msgs[error]);
}

static const char	*get_defined_error_msg(t_defined_errors error)
{
	const char	*error_msgs[DEFINED_ERRORS] = {"File operation failed.",
		"An error occurred in the minilibx library.",
		"XPM file format is invalid or corrupted.",
		"Internal error.",
		"Invalid window size.",
		"Invalid field of view or window width."};

	return (error_msgs[error]);
}
