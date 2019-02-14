#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include "coloring.h"

void	initPalette();

uint32_t	rgb_pack(uint8_t r, uint8_t g, uint8_t b);

#define PALETTE_COLOR_COUNT 1536

uint32_t depth_to_color(uint16_t depth, int depth_max)
{
	static bool		init_done;
	static uint32_t	palette[PALETTE_COLOR_COUNT];
	uint32_t		color;
	uint32_t		chunk_mask;

	if (init_done == false)
	{
		initPalette(palette);
		init_done = true;
	}

	chunk_mask = (depth >> 15) * 0x22000000u;
	depth &= ~(1 << 15);
//		int color = (255.f * (depth / (float)depth_max));
//	return (color << 8);
//		surface.pixels[i] = color << 8;
//		assert(depth <= config.depth_max);
	float normalized = (depth / (float)depth_max);
//		color_index *= 2;
//		printf("%f -> %f\n", normalized, log2f(normalized));
	normalized = sqrtf(normalized);
//		normalized = (depth > 0) * log2f(normalized);
	int color_index = (int) (normalized * (PALETTE_COLOR_COUNT - 1));
//	color_index *= 2;
//	color_index *= 2;
//		color_index += color_index < 0;
	color_index %= PALETTE_COLOR_COUNT;
	color = palette[color_index] * (depth != 0);
	color |= chunk_mask;
	return color;
}

void	draw_color_region(t_config config, t_rect rect, t_surface surface, t_surface16 iter_frame)
{
	int			x;
	int			y;
	if (rect.size.x == 0 || rect.size.y == 0)
		return;
	printf("draw_color_region\n\0origin: %4g %4g,  size: %4g %4g\n\n", rect.origin.x, rect.origin.y, rect.size.x, rect.size.y);
	y = (int)rect.origin.y;
	while (y < (rect.origin.y + rect.size.y))
	{
		x = (int)rect.origin.x;
		while (x < (rect.origin.x + rect.size.x))
		{
			int i = (int)(y * iter_frame.size.x + x);
			uint16_t depth = iter_frame.iter[i];
			surface.pixels[i] = depth_to_color(depth, config.depth_max);
			x++;
		}
		y++;
	}
}

void draw_color(t_surface surface, t_surface16 iter_frame, t_config config)
{
	int			i;

	i = 0;
	while (i < (iter_frame.size.x * iter_frame.size.y))
	{
		uint16_t depth = iter_frame.iter[i];
		surface.pixels[i] = depth_to_color(depth, config.depth_max);
		i++;
	}
}

void	initPalette(int *palette) {
	int i;
	for (i=0;i<128;i++) {
		palette[i] = rgb_pack(254, 0, 127-i);
		// 254, 0, 0
		palette[i+128] = rgb_pack(254, i, 0);
		// 254, 127, 0
		palette[i+256] = rgb_pack(254, 127+i, 0);
		// 254, 254, 0
		palette[i+384] = rgb_pack(254-i, 254, 0);
		// 127, 254, 0
		palette[i+512] = rgb_pack(127-i, 254, 0);
		// 0, 254, 0
		palette[i+640] = rgb_pack(0, 254, i);
		// 0, 254, 127
		palette[i+768] = rgb_pack(0, 254, 127+i);
		// 0, 254, 254
		palette[i+896] = rgb_pack(0, 254-i, 254);
		// 0, 127, 254
		palette[i+1024] = rgb_pack(0, 127-i, 254);
		// 0, 0, 254
		palette[i+1152] = rgb_pack(i, 0, 254);
		// 127, 0, 254
		palette[i+1280] = rgb_pack(127+i, 0, 254);
		// 254, 0, 254
		palette[i+1408] = rgb_pack(254, 0, 254-i);
		// 254, 0, 127
		/*palette[i+1536] = rgb_pack(0, 0, 127-i);
												// 254, 0, 0*/
	}
}

# define R_SHIFT (16u)
# define G_SHIFT (8u)
# define B_SHIFT (0u)

uint32_t	rgb_pack(uint8_t r, uint8_t g, uint8_t b)
{
	uint32_t res;

	res = 0;
	res += (uint8_t)r << R_SHIFT;
	res += (uint8_t)g << G_SHIFT;
	res += (uint8_t)b << B_SHIFT;
	return (res);
}