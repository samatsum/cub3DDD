#include "utils/utils.h"       /* 自身のプロトタイプ宣言のため */

/* ************************************************************************** */
void*
	ft_memmove(void* dst, const void* src, unsigned long len);

/* ************************************************************************** */
// src から dst へ len バイトをコピーする。領域が重なっても壊れないよう、dst>src のときは末尾から、
// それ以外は先頭からコピーして、まだ読んでいないバイトを上書きしない順序を選ぶ。
// dst か src が NULL なら何もせず NULL を返し、正常時はコピー先 dst を返す
void*
	ft_memmove(void* dst, const void* src, unsigned long len)
{
	int	i;

	if (!dst || !src) {
		return (NULL);
	}
	if (dst > src) {
		i = (int)len - 1;
		while (i >= 0) {
			*(char*)(dst + i) = *(char*)(src + i);
			i--;
		}
	} else {
		i = 0;
		while (i < (int)len) {
			*(char*)(dst + i) = *(char*)(src + i);
			i++;
		}
	}
	return (dst);
}
