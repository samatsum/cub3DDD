/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 13:28:44 by hyamamot          #+#    #+#             */
/*   Updated: 2026/02/20 13:28:45 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

char	*ft_strjoin(char *s1, char *s2)
{
	int		len1;
	int		len2;
	char	*str;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	str = (char *)malloc((len1 + len2 + 1));
	if (!str)
		return (NULL);
	ft_memset(str, 0, len1 + len2 + 1);
	ft_strcpy(str, s1);
	ft_strcpy(str + len1, s2);
	return (str);
}
