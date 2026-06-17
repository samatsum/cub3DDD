#ifndef RAYCAST_H
# define RAYCAST_H

# include "utils/utils.h"
# include "config/config.h"

/* ************************************************************************** */
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
	int			is_door;
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
struct s_world;

/* ************************************************************************** */
void
	collect_spawns(t_config* config);
void
	apply_spawn(t_config* config, t_camera* camera, t_spawn_point* sp);
int
	pick_spawn_index(t_config* config, char const* allowed, unsigned int* seed);
int
	pick_spawn_indices(t_config* config, char const* allowed, unsigned int* seed, int* out, int want);
int
	move_camera(t_camera* c, struct s_config* config, struct s_world* world, int direction, double time_mult);
int
	move_perp_camera(t_camera* c, struct s_config* config, struct s_world* world, int direction, double time_mult);
int
	rotate_camera(t_camera* c, struct s_config* config, int dir, double time_mult);
int
	wall_direction(t_raysult* ray);
void
	ray_cast(t_camera* camera, struct s_config* config, t_raysult* ray, double cam_x);
double
	ray_distance(t_camera* camera, t_raysult* ray);
void
	make_tables(struct s_game* game);
int
	spawn_marker_slot(char c);
	
#endif
