#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include "coloring.h"

void	initPalette();

uint32_t	rgb_pack(uint8_t r, uint8_t g, uint8_t b);

#define PALETTE_COLOR_COUNT 1536

void app_render_colors(t_surface surface, t_surface16 iter_frame, t_config config)
{
	int			i;
	static bool	init_done;
	static uint32_t	palette[PALETTE_COLOR_COUNT];
	if (init_done == false)
	{
		initPalette(palette);
		init_done = true;
	}
	i = 0;
	while (i < (iter_frame.size.x * iter_frame.size.y))
	{
		uint16_t depth = iter_frame.iter[i];
//		if (depth > config.depth_max)
//			printf("%i at %i\n", depth, i);

//		int color = (255.f * (depth / (float)config.depth_max));
//		surface.pixels[i] = color << 8;
//		assert(depth <= config.depth_max);
		float normalized = (depth / (float)config.depth_max);
//		color_index *= 2;
//		printf("%f -> %f\n", normalized, log2f(normalized));
		normalized = sqrtf(normalized);
//		normalized = (depth > 0) * log2f(normalized);
		int color_index = (int) (normalized * (PALETTE_COLOR_COUNT - 1));
		color_index *= 2;
		color_index *= 2;
//		color_index += color_index < 0;
		color_index %= PALETTE_COLOR_COUNT;
		surface.pixels[i] = palette[color_index] * (depth != 0);
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