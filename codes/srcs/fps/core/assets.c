#include "core/core.h"             /* t_game 定義・自身のプロトタイプのため */
#include "../minilibx-linux/mlx.h" /* mlx_xpm_file_to_image, mlx_get_data_addr 用 */
#include "tuning.h"                /* DEATH_TEX_PATH, DOOR_TEX_PATH 用 */

/* ************************************************************************** */
void
	load_player_assets(t_game* game);
static void
	load_one_tex(void* mlx_ptr, t_tex* tex, char const* path_src);

/* ************************************************************************** */
// プレイヤー視点のアセット（武器/手・死亡画面・扉）のテクスチャをまとめて読み込む。1枚ぶんの
// 読込は load_one_tex に委譲し、ここでは対象のパス一覧を並べて順に渡すだけ（finish_init から
// 切り出し）。読み込み失敗は致命とせず、その武器/演出/扉が描画されないだけに留める
void
	load_player_assets(t_game* game)
{
	const char*	paths[WEAPON_TEX_COUNT];
	int			i;

	paths[WTEX_PISTOL_IDLE] = "textures/arm/Arm_pistol_static.xpm";
	paths[WTEX_PISTOL_SHOOT] = "textures/arm/Arm_pistol_shoot.xpm";
	paths[WTEX_PISTOL_RECOIL] = "textures/arm/Arm_pistol_recoil.xpm";
	paths[WTEX_FLASHLIGHT] = "textures/arm/Arm_flashlight.xpm";
	paths[WTEX_HAND_LEFT] = "textures/arm/Arm_lefthand.xpm";
	paths[WTEX_HAND_RIGHT] = "textures/arm/Arm_righthand.xpm";
	i = 0;
	while (i < WEAPON_TEX_COUNT) {
		load_one_tex(game->window.ptr, &game->assets.weapon_tex[i], paths[i]);
		i++;
	}
	load_one_tex(game->window.ptr, &game->assets.death_tex, DEATH_TEX_PATH);
	load_one_tex(game->window.ptr, &game->assets.door_tex, DOOR_TEX_PATH);
}

/* ************************************************************************** */
// テクスチャ1枚を読み込む。パスを strdup し、失敗時は何もせず return（tex は呼び出し前の
// NULL のまま＝非致命）。これにより NULL パスを mlx_xpm_file_to_image へ渡す事故を防ぐ。
// 画像化に成功したときだけ data アドレスを取得する。3系統(武器/死亡/扉)で共通化した門番
static void
	load_one_tex(void* mlx_ptr, t_tex* tex, char const* path_src)
{
	tex->path = ft_strdup(path_src);
	if (!tex->path) {
		return ;
	}
	tex->tex = mlx_xpm_file_to_image(mlx_ptr, tex->path, &tex->width, &tex->height);
	if (tex->tex) {
		tex->ptr = mlx_get_data_addr(tex->tex, &tex->bpp, &tex->size_line, &tex->endian);
	}
}
