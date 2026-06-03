/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 13:16:59 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/03 14:31:10 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAYCAST_H
# define RAYCAST_H

# include "utils/utils.h"

/* ************************************************************************** */
# define MOVE_SPEED			0.11
# define COLLISION_MARGIN	0.00001

/* ************************************************************************** */
typedef struct s_camera
{
	t_pos		pos;
	t_pos		dir;
	t_pos		x_dir;
	t_pos		plane;
}				t_camera;

typedef struct s_raysult
{
	int			column;
	int			row;
	double		distance;
	int			direction;
	int			side;
	int			height;
	t_pos		ray_pos;
	t_pos		ray_dir;
	t_pos		map_pos;
	t_pos		side_dist;
	t_pos		delta_dist;
	t_pos		step;
	double		wall_x;
	t_pos		floor_wall;
	t_pos		c_floor;
}				t_raysult;

struct s_game;
struct s_config;

/* ************************************************************************** */
void
	find_start_pos(struct s_config* config, t_camera* camera);
void
	find_start_angle(struct s_config* config, t_camera* camera);
int
	move_camera(t_camera* c, struct s_config* config, int direction);
int
	move_perp_camera(t_camera* c, struct s_config* config, int direction);
int
	rotate_camera(t_camera* c, double* cos_arr, double* sin_arr, int dir);
int
	wall_direction(t_raysult* ray);
void
	ray_cast(t_camera* camera, struct s_config* config, t_raysult* ray, double cam_x);
double
	ray_distance(t_camera* camera, t_raysult* ray);
void
	make_tables(struct s_game* game);

#endif
