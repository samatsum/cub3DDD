#include "utils/utils.h"       /* 自身のプロトタイプ宣言と malloc のため */

/* ************************************************************************** */
char*
	ft_itoa(int n);
static int
	ft_abs(int nbr);
static void
	ft_strrev(char* str);

/* ************************************************************************** */
// 整数 n を10進文字列へ変換して返す（malloc 失敗時は NULL）。下位の桁から順に書き込んでから
// 全体を反転する方式。負数は各桁を ft_abs(n%10) で絶対値化して積み、最後に '-' を付ける。
// n%10 を毎回絶対値化することで、-2147483648(INT_MIN)を符号反転して桁を取り誤る事故を避ける。
// バッファは符号ぶん(is_neg)を加えた固定長を確保し、未使用領域は 0 で埋めて終端も兼ねる
char*
	ft_itoa(int n)
{
	char*	str;
	size_t	length;
	int		is_neg;
	int		i;

	is_neg = (n < 0);
	str = (char*)malloc((11 + is_neg) * sizeof(*str));
	if (!str) {
		return (NULL);
	}
	i = 0;
	while (i < (12 + is_neg)) {
		str[i++] = 0;
	}
	if (n == 0) {
		str[0] = '0';
	}
	length = 0;
	while (n != 0) {
		str[length++] = '0' + ft_abs(n % 10);
		n = (n / 10);
	}
	if (is_neg) {
		str[length] = '-';
	}
	ft_strrev(str);
	return (str);
}

/* ************************************************************************** */
// 整数の絶対値を返す（負なら符号反転、非負ならそのまま）
static int
	ft_abs(int nbr)
{
	if (nbr < 0) {
		return (-nbr);
	} else {
		return (nbr);
	}
}

/* ************************************************************************** */
// 文字列を其の場(in-place)で前後反転する。前半と後半の文字を1組ずつ交換し、中央は触れない
static void
	ft_strrev(char* str)
{
	size_t	length;
	size_t	i;
	char	tmp;

	length = ft_strlen(str);
	i = 0;
	while (i < length / 2) {
		tmp = str[i];
		str[i] = str[length - i - 1];
		str[length - i - 1] = tmp;
		i++;
	}
}
