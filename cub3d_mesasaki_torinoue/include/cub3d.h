/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toruinoue <toruinoue@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 14:53:57 by torinoue          #+#    #+#             */
/*   Updated: 2025/11/04 06:54:05 by toruinoue        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../lib/ft_printf_fd/ft_printf_fd.h"
# include "../lib/libft/libft.h"
# ifdef __APPLE__
#  include "../lib/minilibx_opengl_20191021/mlx.h"
# else
#  include "../lib/minilibx-linux/mlx.h"
# endif
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <math.h>
# include <stdbool.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <stdio.h>
// Include separated header files
# include "cub3d_macros.h"
# ifdef __APPLE__
#  include "cub3d_keymap_MAC.h"
# else
#  include "cub3d_keymap_linux.h"
# endif
# include "cub3d_types.h"
# include "cub3d_functions.h"

// # include <stdio.h>

#endif