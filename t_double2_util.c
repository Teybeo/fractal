/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_double2_util.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/23 17:37:58 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/02/23 17:37:59 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_double2.h"

/*
** The source space origin is ommited as it is assumed to be (0, 0)
*/

void		double2_remap(t_double2 *this, t_double2 src_size,
		t_double2 dst_size, t_double2 dst_origin)
{
	this->x = dst_origin.x + (this->x / src_size.x) * dst_size.x;
	this->y = dst_origin.y + (this->y / src_size.y) * dst_size.y;
}

t_double2	get_center(t_double2 min, t_double2 max)
{
	return (t_double2){
			(max.x - min.x) / 2 + min.x,
			(max.y - min.y) / 2 + min.y
	};
}
