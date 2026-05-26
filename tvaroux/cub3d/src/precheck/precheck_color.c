/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   precheck_color.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomvaroux <tomvaroux@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 12:46:11 by tomvaroux         #+#    #+#             */
/*   Updated: 2026/03/28 12:46:13 by tomvaroux        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "precheck.h"

int	get_color_idx(char *line)
{
	if (line[0] == 'F' && line[1] == ' ')
		return (0);
	if (line[0] == 'C' && line[1] == ' ')
		return (1);
	return (-1);
}

static char	*skip_spaces(char *s)
{
	while (*s == ' ')
		s++;
	return (s);
}

static int	parse_value(char **s)
{
	long	n;
	int		has_digit;

	n = 0;
	has_digit = 0;
	*s = skip_spaces(*s);
	while (**s >= '0' && **s <= '9')
	{
		has_digit = 1;
		n = n * 10 + (**s - '0');
		if (n > 255)
			return (1);
		(*s)++;
	}
	*s = skip_spaces(*s);
	if (!has_digit)
		return (1);
	if (**s && **s != ',' && **s != '\n')
		return (1);
	return (0);
}

static int	parse_comma(char **s)
{
	if (**s != ',')
		return (1);
	(*s)++;
	return (0);
}

int	check_color_line(char *line, t_precheck *p)
{
	int		idx;
	char	*s;

	idx = get_color_idx(line);
	if (idx < 0 || p->color[idx])
		return (1);
	s = skip_spaces(line + 1);
	if (parse_value(&s) || parse_comma(&s)
		|| parse_value(&s) || parse_comma(&s)
		|| parse_value(&s))
		return (1);
	s = skip_spaces(s);
	if (*s != '\0' && *s != '\n')
		return (1);
	p->color[idx] = 1;
	return (0);
}
