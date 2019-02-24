/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coloring.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/23 17:32:07 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/02/24 18:32:49 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_COLORING_H
# define FRACTOL_COLORING_H

# include <stdint.h>
# include "t_double2.h"
# include "drawing.h"

struct			s_surface
{
	uint32_t	*pixels;
	t_double2	size;
};
typedef struct s_surface	t_surface;

# define PALETTE_COLOR_COUNT 4096

void			draw_color(t_config config,
		t_surface surface, t_surface16 iter_frame);
void			draw_color_region(t_config cfg,
		t_surface surface, t_surface16 iter_frame, t_rect rect);
void			set_palette(uint32_t *palette, int gradient_type);

#endif
