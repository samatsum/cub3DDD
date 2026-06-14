#include <math.h>

#include "config/config.h"
#include "core/core.h"
#include "core/collision.h"
#include "enemy/enemy.h"
#include "enemy/enemy_utils.h"

/* ************************************************************************** */
void
	step_enemy(t_enemy* cur, t_game* game, double delta_time, double speed_mult);

/* ************************************************************************** */
// 記録済みの方向(dir_angle)へ、壁と他エンティティを避けつつ指定倍率で1フレーム移動する。
// 基準速度は敵専用の enemy_speed（.cub の ES で可変。プレイヤーの move_speed とは独立）
void
	step_enemy(t_enemy* cur, t_game* game, double delta_time, double speed_mult)
{
	double	time_mult;
	double	speed;
	double	move_x;
	double	move_y;
	t_pos	next_pos;

	time_mult = calc_time_mult(delta_time);
	speed = game->config.enemy_speed * speed_mult * time_mult;
	move_x = cos(cur->dir_angle) * speed;
	move_y = sin(cur->dir_angle) * speed;
	set_pos(&next_pos, cur->sprite->pos.x + move_x, cur->sprite->pos.y);
	if (IN_MAP(next_pos, game->config) && !IS_BLOCKING(MAP(next_pos, game->config)) && !is_blocked_by_entities(&cur->sprite->pos, &next_pos, game, cur->sprite)) {
		cur->sprite->pos.x += move_x;
	}
	set_pos(&next_pos, cur->sprite->pos.x, cur->sprite->pos.y + move_y);
	if (IN_MAP(next_pos, game->config) && !IS_BLOCKING(MAP(next_pos, game->config)) && !is_blocked_by_entities(&cur->sprite->pos, &next_pos, game, cur->sprite)) {
		cur->sprite->pos.y += move_y;
	}
}
