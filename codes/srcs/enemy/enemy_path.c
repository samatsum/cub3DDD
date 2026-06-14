#include <stdlib.h>

#include "config/config.h"
#include "enemy/enemy_utils.h"

/* ************************************************************************** */
int
	bfs_next_step(t_config* config, int sx, int sy, int gx, int gy, t_pos* next);
int
	bfs_to_nearest_patrol(t_config* config, int sx, int sy, t_pos* next);
static int
	cell_walkable(t_config* config, int x, int y);

/* ************************************************************************** */
// BFSで始点から終点への最短経路を探索し、始点の次に進むべき1マスを返す
int
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
// BFSで始点から最も近い Pロード セルを探し、始点の次に進むべき1マスを返す
int
	bfs_to_nearest_patrol(t_config* config, int sx, int sy, t_pos* next)
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
	came[start_idx] = start_idx;
	head = 0;
	tail = 0;
	queue[tail++] = start_idx;
	goal_idx = -1;
	while (head < tail) {
		cur_idx = queue[head++];
		if (config->map.flags[cur_idx] & CELL_PATROL) {
			goal_idx = cur_idx;
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
	if (goal_idx == -1) {
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
