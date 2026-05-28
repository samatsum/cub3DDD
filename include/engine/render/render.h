#ifndef RENDER_H
# define RENDER_H

# include "types.h"

/* screen.c / window management */
int				init_window(t_window *window, t_config *config);
int				clear_window(t_window *window);
void			update_window(t_window *w, int options, int collected, int to_collect);
void			render_frame(struct s_game *game);
void			update_screen(t_game *game);

/* image operations */
int				init_image(t_window *window, t_image *img);
void			destroy_image(t_window *window, t_image *img);

/* draw.c / draw_wall.c / draw_sky_floor.c */
void			draw_pixel(t_window *w, t_pos *pos, int color);
int				draw_vertical_line(t_window *window, t_pos *start, int length, int color);
int				draw_rectangle(t_window *window, t_pos *p1, t_pos *p2, int color);
int				draw_string(t_window *window, t_pos *s_pos, char *str, int color);
void			draw_wall(t_render *rnd, t_raysult *ray);          /* 修正 */
void			draw_sky_floor(t_render *rnd, t_raysult *ray);     /* 修正 */

/* sprite.c / sprite_utils.c */
t_sprite		*add_front_sprite(t_sprite **sprites, double distance, t_pos *pos, t_tex *tex);
t_sprite		*add_sorted_sprite(t_sprite **sprites, t_sprite *sprite);
t_sprite		*sort_sprites(t_camera *camera, t_sprite *sprites); /* 修正 */
void			delete_sprite(t_sprite **sprites, t_pos *pos);
void			draw_sprites(t_render *rnd, t_sprite *sprites);
void			clear_sprites(t_sprite **sprites);

#endif
