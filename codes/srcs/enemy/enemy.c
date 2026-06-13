#include <math.h>
#include <stdlib.h>

#include "config/config.h"
#include "core/core.h"
#include "core/collision.h"
#include "enemy/enemy.h"
#include "../minilibx-linux/mlx.h"
#include "tuning.h"

/* ************************************************************************** */
# define M_PI 3.14159265358979323846

/* ************************************************************************** */
// 敵がプレイヤーを検知する正面視野の半角(ラジアン)。M_PI/8 = ±22.5°で、
// 8方向スプライトの「正面」表示と同じ画角に揃えている
# define ENEMY_FOV_HALF		(M_PI / 8.0)

// 敵がプレイヤーを検知できる最大距離(マス)。本来は無制限の想定だが、
// デバッグ用に上限を設けている。実質無制限にしたい場合は十分大きな値にする
# define ENEMY_SIGHT_RANGE	100.0

// 視線(LOS)判定でレイを1回に進める距離(マス)。小さいほど薄い壁も検出できる
# define ENEMY_LOS_STEP		0.05

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
static int
	enemy_sees_player(t_enemy* cur, t_game* game, double target_angle);
static int
	has_line_of_sight(t_pos* from, t_pos* to, t_config* config);
static void
	move_enemy(t_enemy* cur, t_game* game, double delta_time);
static void
	track_player(t_enemy* cur, t_game* game, double delta_time);
static int
	bfs_next_step(t_config* config, int sx, int sy, int gx, int gy, t_pos* next);
static int
	cell_walkable(t_config* config, int x, int y);
static void
	step_enemy(t_enemy* cur, t_game* game, double delta_time);
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
// 毎フレーム検知判定を行い、追跡タイマー更新・移動・テクスチャ更新を実行する
void
	update_enemies(t_game* game, double delta_time)
{
	t_enemy*	cur;
	double		dx;
	double		dy;
	double		target_angle;

	cur = game->world.enemies;
	while (cur) {
		dx = game->camera.pos.x - cur->sprite->pos.x;
		dy = game->camera.pos.y - cur->sprite->pos.y;
		target_angle = atan2(dy, dx);
		if (enemy_sees_player(cur, game, target_angle)) {
			cur->track_timer = game->config.enemy_track_seconds;
		}
		move_enemy(cur, game, delta_time);
		update_texture(cur, game, target_angle);
		cur = cur->next;
	}
}

/* ************************************************************************** */
// 距離・正面視野(FOV)・視線(LOS)の3条件でプレイヤーを検知できるか判定する
static int
	enemy_sees_player(t_enemy* cur, t_game* game, double target_angle)
{
	double	dx;
	double	dy;
	double	diff;

	dx = game->camera.pos.x - cur->sprite->pos.x;
	dy = game->camera.pos.y - cur->sprite->pos.y;
	if (hypot(dx, dy) > ENEMY_SIGHT_RANGE) {
		return (0);
	}
	diff = target_angle - cur->dir_angle;
	while (diff <= -M_PI) {
		diff += 2.0 * M_PI;
	}
	while (diff > M_PI) {
		diff -= 2.0 * M_PI;
	}
	if (fabs(diff) > ENEMY_FOV_HALF) {
		return (0);
	}
	return (has_line_of_sight(&cur->sprite->pos, &game->camera.pos, &game->config));
}

/* ************************************************************************** */
// 始点から終点まで一定間隔でサンプリングし、間に遮蔽物が無いかを判定する
static int
	has_line_of_sight(t_pos* from, t_pos* to, t_config* config)
{
	t_pos	point;
	t_pos	step;
	double	dist;
	double	steps;
	int		i;

	dist = hypot(to->x - from->x, to->y - from->y);
	steps = dist / ENEMY_LOS_STEP;
	if (steps < 1.0) {
		return (1);
	}
	set_pos(&step, (to->x - from->x) / steps, (to->y - from->y) / steps);
	copy_pos(&point, from);
	i = 0;
	while (i < (int)steps) {
		point.x += step.x;
		point.y += step.y;
		if (!IN_MAP(point, *config) || IS_BLOCKING(MAP(point, *config))) {
			return (0);
		}
		i++;
	}
	return (1);
}

/* ************************************************************************** */
// 追跡タイマーに従い状態を切り替える。残時間が尽きていれば待機状態へ戻す
static void
	move_enemy(t_enemy* cur, t_game* game, double delta_time)
{
	if (cur->track_timer <= 0.0) {
		cur->state = ENEMY_STATE_IDLE;
		return ;
	}
	cur->track_timer -= delta_time;
	cur->state = ENEMY_STATE_WALK;
	track_player(cur, game, delta_time);
}

/* ************************************************************************** */
// プレイヤーの現在地へBFSで最短経路を求め、次の1マスへ向かって前進する
static void
	track_player(t_enemy* cur, t_game* game, double delta_time)
{
	t_pos	next;
	int		sx;
	int		sy;
	int		gx;
	int		gy;
	double	aim_x;
	double	aim_y;

	sx = (int)floor(cur->sprite->pos.x);
	sy = (int)floor(cur->sprite->pos.y);
	gx = (int)floor(game->camera.pos.x);
	gy = (int)floor(game->camera.pos.y);
	aim_x = game->camera.pos.x;
	aim_y = game->camera.pos.y;
	if (!(sx == gx && sy == gy) && bfs_next_step(&game->config, sx, sy, gx, gy, &next)) {
		aim_x = next.x + 0.5;
		aim_y = next.y + 0.5;
	}
	cur->dir_angle = atan2(aim_y - cur->sprite->pos.y, aim_x - cur->sprite->pos.x);
	step_enemy(cur, game, delta_time);
}

/* ************************************************************************** */
// BFSで始点から終点への最短経路を探索し、始点の次に進むべき1マスを返す
static int
	bfs_next_step(t_config* config, int sx, int sy, int gx, int gy, t_pos* next)
{
	int					cols;
	int					total;
	int					head;
	int					tail;
	int					i;
	int					cur_idx;
	int					start_idx;
	int					goal_idx;
	int					nx;
	int					ny;
	int*				came;
	int*				queue;
	static const int	off_x[4] = {1, -1, 0, 0};
	static const int	off_y[4] = {0, 0, 1, -1};

	cols = config->map.columns;
	total = cols * config->map.rows;
	came = (int*)malloc(sizeof(int) * total);
	queue = (int*)malloc(sizeof(int) * total);
	if (!came || !queue) {
		free(came);
		free(queue);
		return (0);
	}
	i = 0;
	while (i < total) {
		came[i++] = -1;
	}
	start_idx = (sy * cols) + sx;
	goal_idx = (gy * cols) + gx;
	came[start_idx] = start_idx;
	head = 0;
	tail = 0;
	queue[tail++] = start_idx;
	while (head < tail) {
		cur_idx = queue[head++];
		if (cur_idx == goal_idx) {
			break ;
		}
		i = 0;
		while (i < 4) {
			nx = (cur_idx % cols) + off_x[i];
			ny = (cur_idx / cols) + off_y[i];
			if (cell_walkable(config, nx, ny) && came[(ny * cols) + nx] == -1) {
				came[(ny * cols) + nx] = cur_idx;
				queue[tail++] = (ny * cols) + nx;
			}
			i++;
		}
	}
	if (came[goal_idx] == -1) {
		free(came);
		free(queue);
		return (0);
	}
	cur_idx = goal_idx;
	while (came[cur_idx] != start_idx) {
		cur_idx = came[cur_idx];
	}
	set_pos(next, cur_idx % cols, cur_idx / cols);
	free(came);
	free(queue);
	return (1);
}

/* ************************************************************************** */
// 指定セルがマップ範囲内かつ壁・通行不可オブジェクトでない(通行可)かを判定する
static int
	cell_walkable(t_config* config, int x, int y)
{
	if (x < 0 || y < 0 || x >= config->map.columns || y >= config->map.rows) {
		return (0);
	}
	return (!IS_BLOCKING(config->map.data[(y * config->map.columns) + x]));
}

/* ************************************************************************** */
// 記録済みの方向(dir_angle)へ、壁と他エンティティを避けつつ半速で1フレーム移動する
static void
	step_enemy(t_enemy* cur, t_game* game, double delta_time)
{
	double	time_mult;
	double	speed;
	double	move_x;
	double	move_y;
	t_pos	next_pos;

	time_mult = delta_time * TARGET_FPS;
	if (time_mult > MAX_TIME_MULT) {
		time_mult = MAX_TIME_MULT;
	}
	speed = (game->config.move_speed / 2.0) * time_mult;
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
