#ifndef TEXTURE_H
# define TEXTURE_H

# include "types.h"

int				load_textures(t_game *game);
void			clear_textures(t_game *game);
int				shade_color(int color, double divide);
int				distance_shade(int options, int color, double distance);
int				get_tex_color(t_tex *tex, t_pos *pos);

#endif