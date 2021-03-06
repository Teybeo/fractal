/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coloring.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/23 20:25:28 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/02/28 14:16:09 by tdarchiv         ###   ########.fr       */
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
	normalized = sinf(((float)M_PI * normalized) * 0.5f);
	color_index = (int)(normalized * (PALETTE_COLOR_COUNT - 1));
	color_index *= 2;
	color_index %= PALETTE_COLOR_COUNT;
	color = palette[color_index] * (depth != 0);
	return (color);
}

void		draw_color_region(t_config cfg, t_frame frame, t_rect rect)
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
			i = (int)(y * frame.size.x + x);
			depth = frame.iter[i];
			frame.pixels[i] = depth_to_color(depth, cfg.depth_max, cfg.palette);
			x++;
		}
		y++;
	}
}

void		draw_color(t_config cfg, t_frame frame)
{
	uint32_t	color;
	int			i;
	int			x;
	int			y;

	y = -1;
	while (++y < frame.size.y)
	{
		x = -1;
		i = (y * (int)(frame.size.x));
		while (++x < frame.size.x)
		{
			color = depth_to_color(frame.iter[i], cfg.depth_max, cfg.palette);
			if (cfg.show_chunks)
				color |= ((y / cfg.lines_per_chunk) % 2) * 0x22000000u;
			if (cfg.show_palette)
				color = cfg.palette[(int)((x / frame.size.x)
										* PALETTE_COLOR_COUNT)];
			frame.pixels[i] = color;
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
