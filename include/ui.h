#ifndef UI_H
# define UI_H

# include "types.h"

void			clear_ui(t_window *window);
void			update_ui(t_render *rnd);
void			write_ui_text(t_window *w, int collected, int to_collect);
void			display_crosshair(t_game *game);
void			check_quest(t_game *game);
void			count_items(t_game *game);

#endif