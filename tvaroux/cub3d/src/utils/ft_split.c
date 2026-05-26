/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 13:28:01 by hyamamot          #+#    #+#             */
/*   Updated: 2026/02/20 13:28:02 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

int	count_words(char const *s, char c)
{
	int	cnt;
	int	in_substring;

	cnt = 0;
	in_substring = 0;
	while (*s)
	{
		if (*s != c && in_substring == 0)
		{
			in_substring = 1;
			cnt++;
		}
		else if (*s == c)
			in_substring = 0;
		s++;
	}
	return (cnt);
}

void	free_all(char **result, int j)
{
	while (j >= 0)
	{
		free(result[j]);
		j--;
	}
	free(result);
}

int	process_word(char const *s, char c, char **result)
{
	int	i;
	int	j;
	int	start;
	int	is_terminal;

	i = 0;
	j = 0;
	is_terminal = 0;
	start = -1;
	while (s[i])
	{
		if (s[i] != c && start < 0)
			start = i;
		if ((s[i] == c || s[i + 1] == '\0') && start >= 0)
		{
			is_terminal = (s[i + 1] == '\0' && s[i] != c);
			result[j] = ft_substr(s, start, i - start + is_terminal);
			if (!result[j++])
				return (-1);
			start = -1;
		}
		i++;
	}
	return (j);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	int		word_count;

	if (!s)
		return (NULL);
	word_count = count_words(s, c);
	result = (char **)malloc(sizeof(char *) * (word_count + 1));
	if (!result)
		return (NULL);
	if (process_word(s, c, result) == -1)
	{
		free_all(result, word_count);
		return (NULL);
	}
	result[word_count] = NULL;
	return (result);
}
