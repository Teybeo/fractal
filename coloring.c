/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coloring.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/23 20:25:28 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/02/23 21:01:39 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coloring.h"

#include "gradient.h"

#include <math.h>

uint32_t	depth_to_color(uint16_t depth, int depth_max, uint32_t *palette)
{
	uint32_t	color;
	float		normalized;
	int			color_index;

	normalized = (depth / (float)depth_max);
//	normalized = 0.5 - cos(M_PI * normalized) / 2;
	normalized = sinf(((float)M_PI * normalized) / 2);
//	normalized = sqrt(normalized);
//	normalized = sqrt(normalized);
	color_index = (int)(normalized * (PALETTE_COLOR_COUNT - 1));
//	color_index *= 2;
//	color_index *= 2;
	color_index %= PALETTE_COLOR_COUNT;
	color = palette[color_index] * (depth != 0);
	return (color);
}

void draw_color_region(t_config cfg, t_surface surface, t_surface16 iter_frame, t_rect rect)
{
	uint16_t	depth;
	int			x;
	int			y;
	int			i;

	if (rect.size.x == 0 || rect.size.y == 0)
		return ;
	y = (int)rect.origin.y;
	while (y < (rect.origin.y + rect.size.y))
	{
		x = (int)rect.origin.x;
		while (x < (rect.origin.x + rect.size.x))
		{
			i = (int)(y * iter_frame.size.x + x);
			depth = iter_frame.iter[i];
			surface.pixels[i] = depth_to_color(depth, cfg.depth_max, cfg.palette);
			x++;
		}
		y++;
	}
}

void draw_color(t_config config, t_surface surface, t_surface16 iter_frame)
{
	uint32_t	color;
	int			i;
	int			x;
	int			y;

	y = -1;
	while (++y < iter_frame.size.y)
	{
		x = -1;
		i = (y * (int)(iter_frame.size.x));
		while (++x < iter_frame.size.x)
		{
			color = depth_to_color(iter_frame.iter[i], config.depth_max, config.palette);
			if (config.show_chunks)
				color |= ((y / config.lines_per_chunk) % 2) * 0x22000000u;
			if (config.show_palette)
				color = config.palette[(int)((x / iter_frame.size.x) * PALETTE_COLOR_COUNT)];
			surface.pixels[i] = color;
			i++;
		}
	}
}

void		set_palette(uint32_t *palette, int gradient_type)
{
	int			i;
	float		percent;
	t_gradient	gradient;

	gradient = create_gradient(gradient_type);
	i = 0;
	while (i < PALETTE_COLOR_COUNT)
	{
		percent = (float)i / PALETTE_COLOR_COUNT;
		palette[i] = get_color_from_gradient(gradient, percent);
		i++;
	}
	destroy_gradient(gradient);
}
