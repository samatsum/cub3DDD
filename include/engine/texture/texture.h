#ifndef TEXTURE_H
# define TEXTURE_H

# include "../../types.h"

/* 前方宣言 */
struct s_window;
struct s_config;

/* texture.c */
int				load_textures(struct s_window *window, t_tex *tex, struct s_config *config);
void			clear_textures(struct s_window *window, t_tex *tex);

/* color.c */
int				shade_color(int color, double divide);
int				distance_shade(int options, int color, double distance);
int				get_tex_color(t_tex *tex, t_pos *pos);

#endif