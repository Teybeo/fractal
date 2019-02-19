#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include "coloring.h"

void	initPalette();

uint32_t	rgb_set_2(uint8_t r, uint8_t g, uint8_t b);

void	init_palette(uint32_t *palette);
void	init_palette_2(uint32_t *palette);

#define PALETTE_COLOR_COUNT 4096

uint32_t depth_to_color(uint16_t depth, int depth_max)
{
	static bool		init_done;
	static uint32_t	palette[PALETTE_COLOR_COUNT];
	uint32_t		color;
	uint32_t		chunk_mask;

	if (init_done == false)
	{
//		init_palette(palette);
		init_palette_2(palette);
		init_done = true;
	}
	chunk_mask = (depth >> 15) * 0x22000000u;
	depth &= ~(1 << 15);
//	return palette[depth % PALETTE_COLOR_COUNT];
	double normalized = (depth / (double)depth_max);
//		color_index *= 2;
//		printf("%f -> %f\n", normalized, log2f(normalized));
	normalized = sqrt(normalized);
//	normalized = sqrt(normalized);
//	normalized *= normalized;
//		normalized = (depth > 0) * log2f(normalized);
	int color_index = (int) (normalized * (PALETTE_COLOR_COUNT - 1));
	color_index *= 2;
	color_index *= 2;
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
		int index = (int)((i / iter_frame.size.x) * PALETTE_COLOR_COUNT);
//		surface.pixels[i] = depth_to_color(index, 0);
		i++;
	}
}

void	init_palette(uint32_t *palette)
{
	int i;

	i = 0;
	while (i < 128)
	{
		palette[i] = rgb_set_2(254, 0, 127 - i);
		// 254, 0, 0
		palette[i+128] = rgb_set_2(254, i, 0);
		// 254, 127, 0
		palette[i+256] = rgb_set_2(254, 127 + i, 0);
		// 254, 254, 0
		palette[i+384] = rgb_set_2(254 - i, 254, 0);
		// 127, 254, 0
		palette[i+512] = rgb_set_2(127 - i, 254, 0);
		// 0, 254, 0
		palette[i+640] = rgb_set_2(0, 254, i);
		// 0, 254, 127
		palette[i+768] = rgb_set_2(0, 254, 127 + i);
		// 0, 254, 254
		palette[i+896] = rgb_set_2(0, 254 - i, 254);
		// 0, 127, 254
		palette[i+1024] = rgb_set_2(0, 127 - i, 254);
		// 0, 0, 254
		palette[i+1152] = rgb_set_2(i, 0, 254);
		// 127, 0, 254
		palette[i+1280] = rgb_set_2(127 + i, 0, 254);
		// 254, 0, 254
		palette[i+1408] = rgb_set_2(254, 0, 254 - i);
		// 254, 0, 127
		/*palette[i+1536] = rgb_set_2(0, 0, 127-i);
												// 254, 0, 0*/
		i++;
	}
}

struct	s_grad_step
{
	t_rgb		color;
	float	treshold;
};
typedef struct s_grad_step	t_grad_step;

void		init_gradient(t_grad_step *gradient);
void		get_color_interval(t_grad_step *gradient, float percent, t_grad_step *a, t_grad_step *b);
t_rgb		rgb_set(uint8_t r, uint8_t g, uint8_t b);
t_rgb		color_mix(float factor, t_rgb a, t_rgb b);
uint32_t	rgb_pack(t_rgb rgb);

#define GRADIENT_COLOR_COUNT 6

void init_palette_2(uint32_t *palette)
{
	int			i;
	float		percent;
	t_grad_step	gradient[GRADIENT_COLOR_COUNT];
	t_grad_step	a;
	t_grad_step	b;
	float		mix_factor;

	init_gradient(gradient);
	i = 0;
	while (i < PALETTE_COLOR_COUNT)
	{
		percent = (float)i / PALETTE_COLOR_COUNT;
		get_color_interval(gradient, percent, &a, &b);
		mix_factor = (percent - a.treshold) / (b.treshold - a.treshold);
		palette[i] = rgb_pack(color_mix(mix_factor, a.color, b.color));
		i++;
	}
}

void	get_color_interval(t_grad_step *gradient, float percent, t_grad_step *a, t_grad_step *b)
{
	int	i;

	i = 0;
	while (i < (GRADIENT_COLOR_COUNT - 1))
	{
		if (percent >= gradient[i].treshold && percent < gradient[i + 1].treshold)
			break;
		i++;
	}
	*a = gradient[i];
	*b = gradient[i + 1];
}

void	init_gradient(t_grad_step *gradient)
{
	gradient[0].treshold = 0;
	gradient[1].treshold = 0.2;
	gradient[2].treshold = 0.4;
	gradient[3].treshold = 0.6;
	gradient[4].treshold = 0.8;
	gradient[5].treshold = 1;
	gradient[0].color = rgb_set(0, 0, 128);
	gradient[1].color = rgb_set(30, 100, 200);
	gradient[2].color = rgb_set(255, 255, 255);
	gradient[3].color = rgb_set(255, 170, 0);
	gradient[4].color = rgb_set(0, 0, 0);
	gradient[5].color = rgb_set(0, 0, 128);
}

# define R_SHIFT (16u)
# define G_SHIFT (8u)
# define B_SHIFT (0u)


t_rgb	color_mix(float factor, t_rgb a, t_rgb b)
{
	t_rgb output;

	output.r = ((1 - factor) * a.r) + (factor * b.r);
	output.g = ((1 - factor) * a.g) + (factor * b.g);
	output.b = ((1 - factor) * a.b) + (factor * b.b);
	return (output);
}


t_rgb		rgb_set(uint8_t r, uint8_t g, uint8_t b)
{
	t_rgb res;

	res.r = r / 255.f;
	res.g = g / 255.f;
	res.b = b / 255.f;
	return (res);
}

uint32_t	rgb_pack(t_rgb rgb)
{
	uint32_t res;

	res = 0;
	res += (uint8_t)(rgb.r * 255) << R_SHIFT;
	res += (uint8_t)(rgb.g * 255) << G_SHIFT;
	res += (uint8_t)(rgb.b * 255) << B_SHIFT;
	return (res);
}

uint32_t	rgb_set_2(uint8_t r, uint8_t g, uint8_t b)
{
	uint32_t res;

	res = 0;
	res += (uint8_t)r << R_SHIFT;
	res += (uint8_t)g << G_SHIFT;
	res += (uint8_t)b << B_SHIFT;
	return (res);
}