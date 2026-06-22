#include "utils/utils.h"       /* 自身のプロトタイプ宣言のため */

/* ************************************************************************** */
int
	ft_write_int(char* buf, int val, int start);
int
	ft_write_str(char* buf, char* str, int start);

/* ************************************************************************** */
// バッファ buf の start 位置から整数 val を10進文字列として書き込み、書き込み後の終端位置を返す。
// まず桁数 length を数え、各桁を下位から所定の位置へ置く。val==0 は '0' 1文字。負数は想定しない
// （UI の数値表示など非負前提）。末尾に '\0' を付ける。境界チェックはしないので buf は十分な容量を要する
int
	ft_write_int(char* buf, int val, int start)
{
	int	length;
	int	tmp;

	length = 1;
	tmp = val;
	while (tmp > 9) {
		length++;
		tmp /= 10;
	}
	if (val == 0) {
		buf[start++] = '0';
	} else {
		tmp = length;
		while (val > 0) {
			buf[start + --tmp] = "0123456789"[val % 10];
			val /= 10;
		}
		start += length;
	}
	buf[start] = 0;
	return (start);
}

/* ************************************************************************** */
// バッファ buf の start 位置から文字列 str をそのまま書き込み、書き込み後の終端位置を返す。
// 末尾に '\0' を付ける。境界チェックはしないので、buf 側で十分な容量を確保しておくこと
int
	ft_write_str(char* buf, char* str, int start)
{
	int	i;

	i = 0;
	while (str[i]) {
		buf[start++] = str[i++];
	}
	buf[start] = 0;
	return (start);
}
