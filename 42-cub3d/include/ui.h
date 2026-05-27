#ifndef UI_H
# define UI_H

# include "types.h"

void			clear_ui(t_window *window);
void			update_ui(t_game *game);
void			write_ui_text(t_game *game);
void			display_crosshair(t_game *game);
void			check_quest(t_game *game);
void			count_items(t_game *game);

#endif