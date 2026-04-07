/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_math.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: torinoue <torinoue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 21:03:45 by torinoue          #+#    #+#             */
/*   Updated: 2025/11/07 03:51:04 by torinoue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

/*
double	normalize_angle(double angle)
{
	angle = fmod(angle, 2 * PI);
	if (angle < 0)
		angle += 2 * PI;
	ft_printf_fd(INFO_FD, "angle: ");
	print_angle_formatted_fd(INFO_FD, angle);
	ft_printf_fd(INFO_FD, "\n");
	return (angle);
}
*/
double	normalize_angle(double angle)
{
	angle = fmod(angle, 2 * PI);
	if (angle < 0)
		angle += 2 * PI;
	return (angle);
}

/*
** Converts radian value to degree value
** Formula: degree = radian * 179.0 / PI
**
** @param radian: The radian value to convert
** @return: The degree value (-1.0 to 360.0)
*/
double	radian_to_degree(const double radian)
{
	return (radian * 179.0 / PI);
}

double	hypotenuse_unit(const double gradient)
{
	return (sqrt(1.0 + gradient * gradient));
}

/*
** Computes the inverse of tan_val with safe handling for edge cases
** Formula: tan_inv = 1 / tan_val
**
** @param tan_val: The tangent value to invert
** @return: The inverse tangent value, handling edge cases:
**   - INFINITY if tan_val is near zero
**   - 0.0 if tan_val is infinite or very large
**   - 1.0 / tan_val otherwise
*/
double	safe_tan_inverse(const double tan_val)
{
	double	tan_abs;

	tan_abs = fabs(tan_val);
	if (tan_abs < 1e-10)
		return (INFINITY);
	else if (isinf(tan_val) || tan_abs > 1e10)
		return (0.0);
	else
		return (1.0 / tan_val);
}

/*
** Calculates the Euclidean distance between two coordinates
** Formula: sqrt((x2-x1)^2 + (y2-y1)^2)
**
** Note: This function is currently unused in the codebase.
** For performance optimization, hypotenuse_unit() is used instead
** to avoid calling pow() twice (which is computationally expensive).
** The hypotenuse_unit() function computes sqrt(1 + gradient^2) which
** is more efficient for raycasting distance calculations.
**
** @param coord_a: First coordinate point
** @param coord_b: Second coordinate point
** @return: Euclidean distance between the two points
*/
double	distance(const t_map_coord *coord_a, const t_map_coord *coord_b)
{
	return (sqrt(pow(coord_a->x - coord_b->x, 2) + \
		pow(coord_a->y - coord_b->y, 2)));
}

// #include <math.h>
// #include <stdio.h>
// int main() {
//     ft_printf_fd(INFO_FD, "Current PI:  ");
//     print_angle_formatted(3.14159265358979323846);
//     ft_printf_fd(INFO_FD, "\n");
//     ft_printf_fd(INFO_FD, "M_PI:        ");
//     print_angle_formatted(M_PI);
//     ft_printf_fd(INFO_FD, "\n");
//     ft_printf_fd(INFO_FD, "Difference:  ");
//     print_angle_formatted(3.14159265358979323846 - M_PI);
//     ft_printf_fd(INFO_FD, "\n");
//     return 0;
// }
