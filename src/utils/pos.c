/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pos.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/31 15:55:48 by samatsum          #+#    #+#             */
/*   Updated: 2026/05/28 15:59:01 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/utils.h"

void
	set_pos(t_pos *pos, double x, double y)
{
	pos->x = x;
	pos->y = y;
}

void
	copy_pos(t_pos *pos, t_pos *org)
{
	pos->x = org->x;
	pos->y = org->y;
}
