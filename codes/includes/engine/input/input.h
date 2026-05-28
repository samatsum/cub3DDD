#ifndef ENGINE_INPUT_HOOKS_H
# define ENGINE_INPUT_HOOKS_H

/* mlxのイベントフック関数群 */
int             main_loop(t_game *game);
int             exit_hook(t_game *game);
int             expose_hook(t_game *game);
int             key_press(int keycode, t_game *game);
int             key_release(int keycode, t_game *game);

#endif