/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coloring.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/23 17:32:07 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/02/28 14:08:25 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_COLORING_H
# define FRACTOL_COLORING_H

# include <stdint.h>
# include "t_double2.h"
# include "config.h"

# define PALETTE_COLOR_COUNT 4096

typedef struct	s_frame
{
	uint16_t	*iter;
	uint32_t	*pixels;
	t_double2	size;
}				t_frame;

void			draw_color(t_config cfg, t_frame frame);
void			draw_color_region(t_config cfg, t_frame frame, t_rect rect);
void			set_palette(uint32_t *palette, int gradient_type);

#endif
