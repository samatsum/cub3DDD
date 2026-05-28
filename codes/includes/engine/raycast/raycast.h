#ifndef ENGINE_RAYCAST_H
# define ENGINE_RAYCAST_H

# include "../../types.h"

# define MOVE_SPEED			0.11
# define COLLISION_MARGIN	0.00001

/* 前方宣言 */
struct s_game;
struct s_config;
struct s_camera;
struct s_raysult;

/* camera.c */
void			find_start_pos(struct s_config *config, struct s_camera *camera);
void			find_start_angle(struct s_config *config, struct s_camera *camera);
int				move_camera(struct s_camera *c, struct s_config *config, int direction);
int				move_perp_camera(struct s_camera *c, struct s_config *config, int direction);
int				rotate_camera(struct s_camera *c, double *cos_arr, double *sin_arr, int dir);

/* raycast.c */
int				wall_direction(struct s_raysult *ray);
void			ray_cast(struct s_camera *camera, struct s_config *config, struct s_raysult *ray, double cam_x);
double			ray_distance(struct s_camera *camera, struct s_raysult *ray);

/* tables.c */
void			make_tables(struct s_game *game);

#endif