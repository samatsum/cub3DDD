#ifndef CORE_H
# define CORE_H

/* Game Options Flags (types.h から移動) */
# define FLAG_UI			0x00000010
# define FLAG_SHADOWS		0x00000100
# define FLAG_CROSSHAIR		0x00001000
# define FLAG_SAVE			0x00000001

/* 究極の依存排除：前方宣言 (types.h をインクルードする必要がなくなる) */
struct s_game;

int		main_loop(struct s_game *game);
void	init_game(struct s_game *game);
int		finish_init(struct s_game *game);
int		exit_game(struct s_game *game, int code);
int		exit_error(struct s_game *game, const char *str);
int		screenshot(struct s_game *game);
void	check_quest(struct s_game *game);
void	count_items(struct s_game *game);

#endif