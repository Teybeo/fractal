/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/23 20:15:28 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/02/28 14:18:35 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "drawing.h"
#include "config.h"
#include "multithreading.h"

#include <math.h>

void	compute_region(t_config cfg, t_rect rect, t_frame frame)
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

int		get_mandelbrot_value(t_double2 c, int depth_max)
{
	t_double2	z;
	int			depth;
	double		z_y_temp;

	depth = 1;
	z = c;
	while (((z.x * z.x) + (z.y * z.y) < 4) && (depth < depth_max))
	{
		z_y_temp = (2 * z.x * z.y) + c.y;
		z.x = (z.x * z.x) - (z.y * z.y) + c.x;
		z.y = z_y_temp;
		depth++;
	}
	if (depth >= depth_max)
		return (0);
	return (depth);
}

int		get_burningship_value(t_double2 c, int depth_max)
{
	t_double2	z;
	int			depth;
	double		z_y_temp;

	depth = 1;
	z = c;
	while (((z.x * z.x) + (z.y * z.y) < 4) && (depth < depth_max))
	{
		z_y_temp = 2 * z.x * z.y;
		z.x = (z.x * z.x) - (z.y * z.y) + c.x;
		z.y = fabs(z_y_temp) + c.y;
		z.x = fabs(z.x);
		depth++;
	}
	if (depth >= depth_max)
		return (0);
	return (depth);
}

int		get_julia_value(t_double2 z_in, int depth_max, t_double2 c)
{
	t_double2	z;
	int			depth;
	double		z_y_temp;

	depth = 1;
	z = z_in;
	while (((z.x * z.x) + (z.y * z.y) < 4) && (depth < depth_max))
	{
		z_y_temp = (2 * z.x * z.y) + c.y;
		z.x = (z.x * z.x) - (z.y * z.y) + c.x;
		z.y = z_y_temp;
		depth++;
	}
	if (depth >= depth_max)
		return (0);
	return (depth);
}
