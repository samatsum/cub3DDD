#include <math.h>          /* cos, sin 関数用 */
#include <stddef.h>        /* NULL 用 */

#include "engine/raycast/raycast.h"
#include "config/config.h" /* MAP等のマクロ展開のため */
#include "core/collision.h"
#include "tuning.h"

/* ************************************************************************** */
int
	move_camera(t_camera* c, t_config* config, struct s_world* world, int direction, double time_mult);
int
	move_perp_camera(t_camera* c, t_config* config, struct s_world* world, int direction, double time_mult);
int
	rotate_camera(t_camera* c, t_config* config, int dir, double time_mult);

/* ************************************************************************** */
// 前後へ移動。壁・通行不可・他エンティティの判定と訪問済みマーキングを行う
int
	move_camera(t_camera* c, t_config* config, struct s_world* world, int direction, double time_mult)
{
	t_pos	n_pos;
	double	actual_speed;

	/* 速度に時間倍率を掛けて、FPSに依存しない移動量を計算 */
	actual_speed = config->move_speed * time_mult;
	copy_pos(&n_pos, &c->pos);
	n_pos.x += (((direction) ? -1 : 1) * (c->dir.x * actual_speed));
	if (IN_MAP(n_pos, *config) && !IS_BLOCKING(MAP(n_pos, *config)) && !is_blocked_by_enemies(&c->pos, &n_pos, world, NULL)) {
		copy_pos(&c->pos, &n_pos);
	}
	copy_pos(&n_pos, &c->pos);
	n_pos.y += (((direction) ? -1 : 1) * (c->dir.y * actual_speed));
	if (IN_MAP(n_pos, *config) && !IS_BLOCKING(MAP(n_pos, *config)) && !is_blocked_by_enemies(&c->pos, &n_pos, world, NULL)) {
		copy_pos(&c->pos, &n_pos);
	}
	if (!IS_COLLECTIBLE(MAP(c->pos, *config)) && !ft_in_set(MAP(c->pos, *config), DIRECTIONS)) {
		MAP(c->pos, *config) = 'A';
	}
	return (1);
}

/* ************************************************************************** */
// 左右へ平行移動。壁・通行不可・他エンティティの判定とマーキングを行う
int
	move_perp_camera(t_camera* c, t_config* config, struct s_world* world, int direction, double time_mult)
{
	t_pos	n_pos;
	double	actual_speed;

	/* 速度に時間倍率を掛けて、FPSに依存しない移動量を計算 */
	actual_speed = config->move_speed * time_mult;
	copy_pos(&n_pos, &c->pos);
	n_pos.x += (((direction) ? -1 : 1) * (c->x_dir.x * actual_speed) + COLLISION_MARGIN);
	if (IN_MAP(n_pos, *config) && !IS_BLOCKING(MAP(n_pos, *config)) && !is_blocked_by_enemies(&c->pos, &n_pos, world, NULL)) {
		copy_pos(&c->pos, &n_pos);
	}
	copy_pos(&n_pos, &c->pos);
	n_pos.y += (((direction) ? -1 : 1) * (c->x_dir.y * actual_speed) + COLLISION_MARGIN);
	if (IN_MAP(n_pos, *config) && !IS_BLOCKING(MAP(n_pos, *config)) && !is_blocked_by_enemies(&c->pos, &n_pos, world, NULL)) {
		copy_pos(&c->pos, &n_pos);
	}
	if (!IS_COLLECTIBLE(MAP(c->pos, *config)) && !ft_in_set(MAP(c->pos, *config), DIRECTIONS)) {
		MAP(c->pos, *config) = 'A';
	}
	return (1);
}

/* ************************************************************************** */
// カメラの視線を回転させ、方向ベクトルと平面ベクトルを更新する
int
	rotate_camera(t_camera* c, t_config* config, int dir, double time_mult)
{
	t_pos	old;
	double	actual_rot;
	double	cos_val;
	double	sin_val;

	/* 回転速度に時間倍率を掛け、可変FPSに対応するため毎回cos/sinを計算する */
	actual_rot = config->rotate_speed * time_mult;
	if (dir == 0) {
		actual_rot = -actual_rot;
	}
	cos_val = cos(actual_rot);
	sin_val = sin(actual_rot);
	copy_pos(&old, &c->dir);
	c->dir.x = (c->dir.x * cos_val) - (c->dir.y * sin_val);
	c->dir.y = (old.x * sin_val) + (c->dir.y * cos_val);
	copy_pos(&old, &c->plane);
	c->plane.x = (c->plane.x * cos_val) - (c->plane.y * sin_val);
	c->plane.y = (old.x * sin_val) + (c->plane.y * cos_val);
	copy_pos(&old, &c->x_dir);
	c->x_dir.x = (c->x_dir.x * cos_val) - (c->x_dir.y * sin_val);
	c->x_dir.y = (old.x * sin_val) + (c->x_dir.y * cos_val);
	return (1);
}
