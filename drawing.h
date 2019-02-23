/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/23 16:59:59 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/02/23 16:59:59 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DRAWING_H
# define DRAWING_H

# include "t_double2.h"
# include "config.h"
# include <stdint.h>

struct	s_surface16
{
	uint16_t	*iter;
	t_double2	size;
};
typedef struct s_surface16	t_surface16;

void	draw_iter_region(t_config cfg, t_rect rect, t_surface16 iter_frame);
int		get_mandelbrot_value(t_double2 c, int depth_max);
int		get_burningship_value(t_double2 c, int depth_max);
int		get_julia_value(t_double2 c, int depth_max, t_double2 z_in);

#endif
