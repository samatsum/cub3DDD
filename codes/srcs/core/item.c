#include "core/core.h"
#include "tuning.h"

/* ************************************************************************** */
void
	check_quest(t_game* game);
void
	count_items(t_game* game);

/* ************************************************************************** */
// プレイヤーの現在位置にあるアイテムを取得したか判定し、状態を更新する
void
	check_quest(t_game* game)
{
	if (IS_COLLECTIBLE(MAP(game->camera.pos, game->config))) {
		MAP(game->camera.pos, game->config) = 'A';
		game->world.collected++;
		delete_sprite(&game->world.sprites, &game->camera.pos);
	}
}

/* ************************************************************************** */
// マップ上に存在する収集アイテムの総数を数える
void
	count_items(t_game* game)
{
	int	i;
	int	j;

	game->world.to_collect = 0;
	i = 0;
	while (i < game->config.map.rows) {
		j = 0;
		while (j < game->config.map.columns) {
			if (IS_COLLECTIBLE(MAP_XY(j, i, game->config))) {
				game->world.to_collect++;
			}
			j++;
		}
		i++;
	}
}
