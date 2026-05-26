/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   precheck_init_finish.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomvaroux <tomvaroux@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 15:33:31 by tomvaroux         #+#    #+#             */
/*   Updated: 2026/03/28 15:44:16 by tomvaroux        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "precheck.h"
#include "utils.h"

void	precheck_init(t_precheck *p)
{
	ft_memset(p->tex, 0, sizeof(p->tex));
	ft_memset(p->color, 0, sizeof(p->color));
	p->player = 0;
	p->map_started = 0;
	p->map_ended = 0;
}

int	finish_precheck(t_precheck *p)
{
	int	i;

	if (!p->map_started || p->player != 1)
		return (1);
	i = 0;
	while (i < 4)
	{
		if (!p->tex[i])
			return (1);
		i++;
	}
	if (!p->color[0] || !p->color[1])
		return (1);
	return (0);
}
