#include "coloring.h"

#include "gradient.h"

#include <math.h>

uint32_t depth_to_color(uint16_t depth, int depth_max, uint32_t *palette)
{
	uint32_t	color;
	uint32_t	chunk_mask;
	float		normalized;
	int			color_index;

	chunk_mask = (depth >> 15) * 0x22000000u;
	depth &= ~(1 << 15);
//	return palette[depth % PALETTE_COLOR_COUNT];
	normalized = (depth / (float)depth_max);
//	normalized = 0.5 - cos(M_PI * normalized) / 2;
	normalized = sinf(((float)M_PI * normalized) / 2);
//	normalized = sqrt(normalized);
//	normalized = sqrt(normalized);
	color_index = (int) (normalized * (PALETTE_COLOR_COUNT - 1));
	color_index *= 2;
	color_index *= 2;
	color_index %= PALETTE_COLOR_COUNT;
	color = palette[color_index] * (depth != 0);
	color |= chunk_mask;
	return color;
}

void	draw_color_region(t_config config, t_rect rect, t_surface surface, t_surface16 iter_frame)
{
	int	x;
	int	y;

	if (rect.size.x == 0 || rect.size.y == 0)
		return;
	y = (int)rect.origin.y;
	while (y < (rect.origin.y + rect.size.y))
	{
		x = (int)rect.origin.x;
		while (x < (rect.origin.x + rect.size.x))
		{
			int i = (int)(y * iter_frame.size.x + x);
			uint16_t depth = iter_frame.iter[i];
			surface.pixels[i] = depth_to_color(depth, config.depth_max, config.palette);
			x++;
		}
		y++;
	}
}

void draw_color(t_surface surface, t_surface16 iter_frame, t_config config)
{
	int	i;

	i = 0;
	while (i < (iter_frame.size.x * iter_frame.size.y))
	{
		uint16_t depth = iter_frame.iter[i];
		surface.pixels[i] = depth_to_color(depth, config.depth_max, config.palette);
//		int index = (int)((i / iter_frame.size.x) * PALETTE_COLOR_COUNT);
//		surface.pixels[i] = depth_to_color(index, 0);
		i++;
	}
}

void set_palette(uint32_t *palette, int gradient_type)
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
