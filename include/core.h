#ifndef CORE_H
# define CORE_H

/* 初期化と終了処理（アプリケーションのライフサイクル管理） */
void            init_game(t_game *game, int save_opt);
int             finish_init(t_game *game);
int             exit_game(t_game *game, int code);
int             exit_error(t_game *game, char const *str);

/* スクリーンショット関連 */
int             screenshot(t_game *game);
int             save_bmp(t_game *game);

#endif