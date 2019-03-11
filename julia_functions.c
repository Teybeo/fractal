/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   julia_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 14:05:11 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/03/11 14:05:13 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractal_functions.h"

#include <math.h>

int			get_julia_value(t_double2 z_in, int depth_max, t_double2 c)
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

int			get_burning_julia_value(t_double2 z_in, int depth_max, t_double2 c)
{
	t_double2	z;
	int			depth;
	double		z_y_temp;

	depth = 1;
	z = z_in;
	while (((z.x * z.x) + (z.y * z.y) < 4) && (depth < depth_max))
	{
		z_y_temp = (2 * z.x * z.y);
		z.x = (z.x * z.x) - (z.y * z.y) + c.x;
		z.y = fabs(z_y_temp) + c.y;
		z.x = fabs(z.x);
		depth++;
	}
	if (depth >= depth_max)
		return (0);
	return (depth);
}

int			get_julia3_value(t_double2 z_in, int depth_max, t_double2 c)
{
	t_double2	z;
	int			depth;
	double		z_y_temp;

	depth = 1;
	z = z_in;
	while (((z.x * z.x) + (z.y * z.y) < 4) && (depth < depth_max))
	{
		z_y_temp = -(z.y * z.y * z.y) + (3 * z.x * z.x * z.y) + c.y;
		z.x = (z.x * z.x * z.x) - (3 * z.x * z.y * z.y) + c.x;
		z.y = z_y_temp;
		depth++;
	}
	if (depth >= depth_max)
		return (0);
	return (depth);
}

int			get_burningjulia3_value(t_double2 z_in, int depth_max, t_double2 c)
{
	t_double2	z;
	int			depth;
	double		z_y_temp;

	depth = 1;
	z = z_in;
	while (((z.x * z.x) + (z.y * z.y) < 4) && (depth < depth_max))
	{
		z_y_temp = -(z.y * z.y * z.y) + (3 * z.x * z.x * z.y) + c.y;
		z.x = (z.x * z.x * z.x) - (3 * z.x * z.y * z.y) + c.x;
		z.y = fabs(z_y_temp) + c.y;
		z.x = fabs(z.x);
		depth++;
	}
	if (depth >= depth_max)
		return (0);
	return (depth);
}
