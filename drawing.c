/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/23 20:15:28 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/03/06 18:36:10 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "drawing.h"

#include "config.h"

void		compute_region(t_config cfg, t_rect rect, t_frame frame)
{
	int			x;
	int			y;
	t_double2	c;
	int			depth;

	y = (int)rect.origin.y;
	while (y < (rect.origin.y + rect.size.y))
	{
		x = (int)rect.origin.x;
		c.y = (y / frame.size.y) * (cfg.z_size.y) + (cfg.z_min.y);
		while (x < (rect.origin.x + rect.size.x))
		{
			c.x = (x / frame.size.x) * (cfg.z_size.x) + (cfg.z_min.x);
			depth = cfg.fractal_fn(c, cfg.depth_max, cfg.z_mouse);
			frame.iter[(int)(y * frame.size.x + x)] = (uint16_t)depth;
			x++;
		}
		y++;
	}
}
