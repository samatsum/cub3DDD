#include "rsp/rsp.h"

/* ************************************************************************** */

t_rsp_result
	rsp_outcome(t_hand a, t_hand b);
t_hand
	rsp_rehand(t_hand current, unsigned int* seed);

/* ************************************************************************** */

// 手aから見た勝敗を返す。Rock=0,Scissors=1,Paper=2 の循環順により、
// (a - b + 3) % 3 が 0=あいこ / 1=aの勝ち / 2=aの負け に一致する
t_rsp_result
	rsp_outcome(t_hand a, t_hand b)
{
	int	diff;

	diff = ((int)a - (int)b + HAND_COUNT) % HAND_COUNT;
	if (diff == 0) {
		return (RSP_DRAW);
	} else if (diff == 1) {
		return (RSP_WIN);
	} else {
		return (RSP_LOSE);
	}
}

/* ************************************************************************** */

// 現在の手以外の2つのうち1つをランダムに返す（必ず手が変わる）。リスポーン時の
// 手変更で使用。seed は呼び出し側が保持する乱数状態で、rand_r によりグローバルな
// 乱数状態に依存せず、テスト時の再現性も確保できる
t_hand
	rsp_rehand(t_hand current, unsigned int* seed)
{
	int	offset;

	offset = 1 + (int)((unsigned int)rand_r(seed) % (HAND_COUNT - 1));
	return ((t_hand)(((int)current + offset) % HAND_COUNT));
}
