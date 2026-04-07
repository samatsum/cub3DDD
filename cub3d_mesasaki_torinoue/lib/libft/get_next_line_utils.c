/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toruinoue <toruinoue@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 20:48:43 by torinoue          #+#    #+#             */
/*   Updated: 2025/10/26 01:29:53 by toruinoue        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*free_and_rest_memory(char **memory)
{
	if (*memory != NULL)
		free(*memory);
	*memory = NULL;
	return (NULL);
}

/* Helper strlen for Get Next Line:
 * - Local function to avoid libft dependency
 * - Used by gnl_strjoin and gnl_strdup
 * - Simple while loop
 */
size_t	gnl_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

/* Modified strdup for Get Next Line:
 * - NULL guard to prevent segfault
 * - Manual loop (no ft_strlcpy dependency)
 * - Returns NULL on error via free_and_rest_memory
 * - Copies partial strings from storage
 */
char	*gnl_strdup(const char *src)
{
	char	*copied_string;
	size_t	i;

	if (src == NULL)
		return (NULL);
	copied_string = (char *)malloc(sizeof(char) * (gnl_strlen(src) + 1));
	if (copied_string == NULL)
		return (free_and_rest_memory((char **)src));
	i = 0;
	while (src[i])
	{
		copied_string[i] = src[i];
		i++;
	}
	copied_string[i] = '\0';
	return (copied_string);
}

/* Modified strjoin for Get Next Line:
 * - Handles NULL for s1 OR s2 (returns non-NULL string)
 * - Returns NULL only when both are NULL
 * - Initial NULL state support (storage starts NULL)
 * - Supports partial string concatenation
 * - Used for storage+buffer concatenation
 */
char	*gnl_strjoin(char const *s1, char const *s2)
{
	size_t	size;
	char	*joined;
	size_t	i;

	if (s1 == NULL && s2 == NULL)
		return (NULL);
	size = 1;
	if (s1 != NULL)
		size += gnl_strlen(s1);
	if (s2 != NULL)
		size += gnl_strlen(s2);
	joined = (char *)malloc(sizeof(char) * size);
	if (joined == NULL)
		return (NULL);
	i = 0;
	if (s1 != NULL)
		while (*s1)
			joined[i++] = *s1++;
	if (s2 != NULL)
		while (*s2)
			joined[i++] = *s2++;
	joined[i] = '\0';
	return (joined);
}

/* Modified to return non-NULL string if s1 or s2 is NULL.
 * This accommodates GNL (Get Next Line) functionality.
 * The code below is the standard strjoin version that
 * does not support GNL but has same behavior as stdlib.
 */

/*
char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	size;
	char	*joined;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	size = ft_strlen(s1) + ft_strlen(s2) + 1;
	joined = (char *)ft_calloc(size, sizeof(char));
	if (joined == NULL)
		return (NULL);
	ft_strlcpy(joined, s1, size);
	ft_strlcat(joined, s2, size);
	return (joined);
}
*/
//if (copied_string == NULL)
//	return (NULL);
//		v
//		v
//		v
//		v
//if (copied_string == NULL)
//	return(free_and_rest_memory((char **)src));
//
//
//void	*xmalloc(size_t size)
//{
//	void *ptr;
//	static int i;
//	//printf ("%d ",i);
//	//if (i++ < 100)
//	if (i == 0)
//	{
//		ptr = malloc(size);
//		return (ptr);
//	}
//	(void)size;
//	ptr = NULL;
//	return (ptr);
//}
