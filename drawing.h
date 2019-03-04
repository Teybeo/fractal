/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/23 16:59:59 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/02/28 14:10:06 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DRAWING_H
# define DRAWING_H

# include "t_double2.h"
# include "config.h"
# include "coloring.h"

void	compute_region(t_config cfg, t_rect rect, t_frame frame);
int		get_mandelbrot_value(t_double2 c, int depth_max);
int		get_burningship_value(t_double2 c, int depth_max);
int		get_julia_value(t_double2 c, int depth_max, t_double2 z_in);

static const t_fractl_fn g_fractal_map[3] =
{
	&get_mandelbrot_value,
	&get_julia_value,
	&get_burningship_value,
};

#endif
