#include "engine/raycast/raycast.h"
#include "config/config.h"

/* ************************************************************************** */

int
	spawn_marker_slot(char c);

/* ************************************************************************** */

// スポーン文字に応じたマーカー(光るオブジェクト)のテクスチャスロットを返す。
// 赤チーム(N/W)は TEX_PAS_2(=OP2)、青チーム(S/E)は TEX_PAS_3(=OP3)。案Q-1 で
// N/S/E/W のマスへ自動配置するマーカーの色分けを一箇所に集約する。想定外の文字は
// -1 を返し、呼び出し側でマーカーを置かない（無効スロットの混入を防ぐ）
int
	spawn_marker_slot(char c)
{
	if (IS_RED_SPAWN(c)) {
		return (TEX_PAS_2);
	} else if (IS_BLUE_SPAWN(c)) {
		return (TEX_PAS_3);
	}
	return (-1);
}
