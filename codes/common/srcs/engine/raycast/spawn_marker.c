#include "engine/raycast/raycast.h"
#include "config/config.h"

/* ************************************************************************** */

int
	spawn_marker_slot(char c);

/* ************************************************************************** */

// スポーン文字に応じたマーカー(光るオブジェクト)のテクスチャスロットを返す。
// 赤チーム(N/W)は TEX_PAS_2(=OP2)、青チーム(S/E)は TEX_PAS_3(=OP3)。案Q-1 で
// N/S/E/W のマスへ自動配置するマーカーの色分けを一箇所に集約する
int
	spawn_marker_slot(char c)
{
	if (IS_RED_SPAWN(c)) {
		return (TEX_PAS_2);
	}
	return (TEX_PAS_3);
}
