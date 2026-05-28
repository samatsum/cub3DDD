#ifndef UI_H
# define UI_H

# include "types.h"

void			update_ui(t_render *rnd);
void			write_ui_text(t_window *w, int collected, int to_collect);
void			display_crosshair(t_window *w);

#endif