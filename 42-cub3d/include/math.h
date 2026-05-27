#ifndef MATH_H
# define MATH_H

# include "types.h"

/* camera.c */
void			find_start_pos(t_config *config, t_camera *camera);
void			find_start_angle(t_config *config, t_camera *camera);
int				move_camera(t_game *game, int direction);
int				move_perp_camera(t_game *game, int direction);
int				rotate_camera(t_game *game, int direction);

/* raycast.c */
int				wall_direction(t_raysult *ray);
void			ray_cast(t_game *game, t_raysult *ray, int column);
double			ray_distance(t_game *game, t_raysult *ray);

/* tables.c */
void			make_tables(t_game *game);

#endif