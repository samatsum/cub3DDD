#include <math.h>
#include <stdlib.h>
#include "core/core.h"
#include "core/collision.h"
#include "enemy/enemy.h"
#include "../minilibx-linux/mlx.h"
#include "tuning.h"

/* ************************************************************************** */
#ifndef M_PI
# define M_PI 3.14159265358979323846
#endif

/* ************************************************************************** */
t_enemy*
	add_enemy(t_enemy** enemies, t_sprite* sprite);
void
	delete_enemy(t_enemy** enemies, t_sprite** sprites, t_sprite* target);
void
	clear_enemies(t_enemy** enemies);
void
	damage_enemy(t_game* game, t_sprite* hit_sprite);
int
	init_enemy_textures(t_game* game);
void
	update_enemies(t_game* game, double delta_time);
static void
	move_enemy(t_enemy* cur, t_game* game, double target_angle, double delta_time);
static void
	update_texture(t_enemy* cur, t_game* game, double target_angle);

/* ************************************************************************** */
// 新しい敵を生成し、リストの先頭に追加する
t_enemy*
	add_enemy(t_enemy** enemies, t_sprite* sprite)
{
	t_enemy*	new_enemy;

	new_enemy = (t_enemy*)malloc(sizeof(*new_enemy));
	if (!new_enemy) {
		return (NULL);
	}
	new_enemy->hp = ENEMY_MAX_HP;
	new_enemy->state = ENEMY_STATE_IDLE;
	new_enemy->dir_angle = 0.0;
	new_enemy->track_timer = 0.0;
	new_enemy->sprite = sprite;
	new_enemy->next = *enemies;
	*enemies = new_enemy;
	return (new_enemy);
}

/* ************************************************************************** */
// 対象のスプライトを持つ敵を検索し、敵リストとスプライトリストの両方から削除する
void
	delete_enemy(t_enemy** enemies, t_sprite** sprites, t_sprite* target)
{
	t_enemy*	current;
	t_enemy*	previous;
	t_enemy*	tmp;

	current = *enemies;
	previous = NULL;
	while (current) {
		if (current->sprite == target) {
			tmp = current;
			if (!previous) {
				*enemies = tmp->next;
			} else {
				previous->next = tmp->next;
			}
			delete_sprite(sprites, &target->pos);
			free(tmp);
			tmp = NULL;
			return ;
		}
		previous = current;
		current = current->next;
	}
}

/* ************************************************************************** */
// 敵リストの全メモリを解放する
void
	clear_enemies(t_enemy** enemies)
{
	t_enemy*	tmp;

	while (*enemies) {
		tmp = (*enemies)->next;
		free(*enemies);
		*enemies = tmp;
	}
	*enemies = NULL;
}

/* ************************************************************************** */
// ヒットしたスプライトから敵を特定し、ダメージを与える
void
	damage_enemy(t_game* game, t_sprite* hit_sprite)
{
	t_enemy*	current;

	current = game->world.enemies;
	while (current) {
		if (current->sprite == hit_sprite) {
			if (current->hp > 0) {
				current->hp -= 1;
				if (current->hp <= 0) {
					delete_enemy(&game->world.enemies, &game->world.sprites, hit_sprite);
				}
			}
			return ;
		}
		current = current->next;
	}
}

/* ************************************************************************** */
// 8方向の敵テクスチャをメモリ上にロードし、描画境界(start/end)を初期化する
int
	init_enemy_textures(t_game* game)
{
	char*	paths[ENEMY_TEX_COUNT];
	int		i;

	paths[0] = "textures/enemy/Enemy_1.xpm";
	paths[1] = "textures/enemy/Enemy_2.xpm";
	paths[2] = "textures/enemy/Enemy_3.xpm";
	paths[3] = "textures/enemy/Enemy_4.xpm";
	paths[4] = "textures/enemy/Enemy_5.xpm";
	paths[5] = "textures/enemy/Enemy_6.xpm";
	paths[6] = "textures/enemy/Enemy_7.xpm";
	paths[7] = "textures/enemy/Enemy_8.xpm";
	i = 0;
	while (i < ENEMY_TEX_COUNT) {
		game->assets.enemy_tex[i].path = ft_strdup(paths[i]);
		game->assets.enemy_tex[i].tex = mlx_xpm_file_to_image(game->window.ptr, game->assets.enemy_tex[i].path, &game->assets.enemy_tex[i].width, &game->assets.enemy_tex[i].height);
		if (!game->assets.enemy_tex[i].tex) {
			return (0);
		}
		game->assets.enemy_tex[i].ptr = mlx_get_data_addr(game->assets.enemy_tex[i].tex, &game->assets.enemy_tex[i].bpp, &game->assets.enemy_tex[i].size_line, &game->assets.enemy_tex[i].endian);
		set_pos(&game->assets.enemy_tex[i].start, 0, 0);
		set_pos(&game->assets.enemy_tex[i].end, game->assets.enemy_tex[i].width, game->assets.enemy_tex[i].height);
		i++;
	}
	return (1);
}

/* ************************************************************************** */
// 毎フレーム敵の視界判定を行い、追跡移動とテクスチャの更新を実行する
void
	update_enemies(t_game* game, double delta_time)
{
	t_enemy*	cur;
	double		dx;
	double		dy;
	double		target_angle;
	double		diff;
	int			diff_idx;

	cur = game->world.enemies;
	while (cur) {
		dx = game->camera.pos.x - cur->sprite->pos.x;
		dy = game->camera.pos.y - cur->sprite->pos.y;
		target_angle = atan2(dy, dx);
		diff = target_angle - cur->dir_angle;
		while (diff < 0.0) {
			diff += 2.0 * M_PI;
		}
		while (diff >= 2.0 * M_PI) {
			diff -= 2.0 * M_PI;
		}
		diff_idx = (int)(floor((diff + (M_PI / 8.0)) / (M_PI / 4.0))) % ENEMY_TEX_COUNT;
		/* 視界（正面）にプレイヤーがいれば追跡タイマーをセットする */
		if (diff_idx == 0) {
			cur->track_timer = game->config.enemy_track_seconds;
		}
		move_enemy(cur, game, target_angle, delta_time);
		update_texture(cur, game, target_angle);
		cur = cur->next;
	}
}

/* ************************************************************************** */
// 追跡中、壁と他エンティティ(プレイヤー/敵)を避けつつ半分の速度で接近する
static void
	move_enemy(t_enemy* cur, t_game* game, double target_angle, double delta_time)
{
	double	time_mult;
	double	speed;
	double	move_x;
	double	move_y;
	t_pos	next_pos;

	if (cur->track_timer > 0.0) {
		cur->track_timer -= delta_time;
		cur->state = ENEMY_STATE_WALK;
		cur->dir_angle = target_angle;
		time_mult = delta_time * TARGET_FPS;
		if (time_mult > MAX_TIME_MULT) {
			time_mult = MAX_TIME_MULT;
		}
		speed = (game->config.move_speed / 2.0) * time_mult;
		move_x = cos(cur->dir_angle) * speed;
		move_y = sin(cur->dir_angle) * speed;
		/* X軸: 壁と他エンティティ(プレイヤー/敵)を避けて移動 */
		set_pos(&next_pos, cur->sprite->pos.x + move_x, cur->sprite->pos.y);
		// "MAP(next_pos, game->config) != '1'" <--> "!IS_BLOCKING(MAP(next_pos, game->config))" で、通過不可能オブジェクトを通過するか否かを決められる。
		if (IN_MAP(next_pos, game->config) && !IS_BLOCKING(MAP(next_pos, game->config)) && !is_blocked_by_entities(&cur->sprite->pos, &next_pos, game, cur->sprite)) {
			cur->sprite->pos.x += move_x;
		}
		/* Y軸: 壁と他エンティティ(プレイヤー/敵)を避けて移動 */
		set_pos(&next_pos, cur->sprite->pos.x, cur->sprite->pos.y + move_y);
		// "MAP(next_pos, game->config) != '1'" <--> "!IS_BLOCKING(MAP(next_pos, game->config))" で、通過不可能オブジェクトを通過するか否かを決められる。
		if (IN_MAP(next_pos, game->config) && !IS_BLOCKING(MAP(next_pos, game->config)) && !is_blocked_by_entities(&cur->sprite->pos, &next_pos, game, cur->sprite)) {
			cur->sprite->pos.y += move_y;
		}
	} else {
		cur->state = ENEMY_STATE_IDLE;
	}
}

/* ************************************************************************** */
// プレイヤーの視点に対する敵の相対角度から、適切な8方向テクスチャを選択する
static void
	update_texture(t_enemy* cur, t_game* game, double target_angle)
{
	double	diff;
	int		diff_idx;
	int		tex_idx;

	diff = target_angle - cur->dir_angle;
	while (diff < 0.0) {
		diff += 2.0 * M_PI;
	}
	while (diff >= 2.0 * M_PI) {
		diff -= 2.0 * M_PI;
	}
	diff_idx = (int)(floor((diff + (M_PI / 8.0)) / (M_PI / 4.0))) % ENEMY_TEX_COUNT;
	tex_idx = (ENEMY_TEX_COUNT - diff_idx) % ENEMY_TEX_COUNT;
	cur->sprite->tex = &game->assets.enemy_tex[tex_idx];
}
