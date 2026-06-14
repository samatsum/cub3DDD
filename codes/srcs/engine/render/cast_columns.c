#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include "engine/render/render.h"
#include "engine/raycast/raycast.h"

/* ************************************************************************** */
// 同時に走らせるワーカースレッドの上限。実行時は min(オンラインCPUコア数,
// この値, 画面幅) を採用し、1 のときはスレッドを使わず逐次実行に落とす
#define RENDER_THREADS_MAX	16

/* ************************************************************************** */
void
	cast_columns(t_render* rnd, double* camera_x);
static int
	worker_count(int columns);
static void*
	cast_worker(void* arg);
static void
	cast_range(t_render* rnd, double* camera_x, int start, int end);

/* ************************************************************************** */
// 画面の列をスレッド数で分割し、各ワーカーに割り当てて並列に描画する（司令塔）
void
	cast_columns(t_render* rnd, double* camera_x)
{
	pthread_t		tid[RENDER_THREADS_MAX];
	t_render_job	jobs[RENDER_THREADS_MAX];
	int				created[RENDER_THREADS_MAX];
	int				cols;
	int				n;
	int				i;

	cols = (int)rnd->w->size.x;
	n = worker_count(cols);
	i = 0;
	while (i < n) {
		jobs[i].rnd = rnd;
		jobs[i].camera_x = camera_x;
		jobs[i].start = (cols * i) / n;
		jobs[i].end = (cols * (i + 1)) / n;
		created[i] = 0;
		if (n > 1 && pthread_create(&tid[i], NULL, &cast_worker, &jobs[i]) == 0) {
			created[i] = 1;
		} else {
			cast_range(rnd, camera_x, jobs[i].start, jobs[i].end);
		}
		i++;
	}
	i = 0;
	while (i < n) {
		if (created[i]) {
			pthread_join(tid[i], NULL);
		}
		i++;
	}
}

/* ************************************************************************** */
// オンラインCPUコア数を基に起動するワーカー数を決める（1コアなら1＝逐次実行）
static int
	worker_count(int columns)
{
	long	cores;
	int		n;

	cores = sysconf(_SC_NPROCESSORS_ONLN);
	if (cores < 1) {
		cores = 1;
	}
	n = (int)cores;
	if (n > RENDER_THREADS_MAX) {
		n = RENDER_THREADS_MAX;
	}
	if (n > columns) {
		n = columns;
	}
	if (n < 1) {
		n = 1;
	}
	return (n);
}

/* ************************************************************************** */
// pthread から呼ばれる入口。担当する列範囲を取り出して描画本体へ委譲する
static void*
	cast_worker(void* arg)
{
	t_render_job*	job;

	job = (t_render_job*)arg;
	cast_range(job->rnd, job->camera_x, job->start, job->end);
	return (NULL);
}

/* ************************************************************************** */
// 指定された列範囲[start,end)だけをレイキャストし、壁・床・天井を描画する
static void
	cast_range(t_render* rnd, double* camera_x, int start, int end)
{
	t_raysult	ray;
	int			i;

	i = start;
	while (i < end) {
		ray.column = i;
		ray_cast(rnd->camera, rnd->config, &ray, camera_x[i]);
		rnd->depth[i] = ray.distance;
		ray.height = fabs(rnd->w->size.y / ray.distance);
		draw_wall(rnd, &ray);
		if (ray.height < rnd->w->size.y) {
			draw_sky_floor(rnd, &ray);
		}
		i++;
	}
}
