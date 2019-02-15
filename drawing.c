#include "drawing.h"
#include "config.h"
#include "multithreading.h"

#include <math.h>

void	draw_iter_region(t_config config, t_rect rect, t_surface16 iter_frame, bool chunk_mask)
{
	int			x;
	int			y;
	t_float2	c;
	int			depth;

//	printf("draw_iter_region\norigin: %4g %4g,  size: %4g %4g\n", rect.origin.x, rect.origin.y, rect.size.x, rect.size.y);
	y = (int)rect.origin.y;
	while (y < (rect.origin.y + rect.size.y))
	{
		x = (int)rect.origin.x;
		c.y = (y / iter_frame.size.y) * (config.z_size.y) + (config.z_min.y);
		while (x < (rect.origin.x + rect.size.x))
		{
			c.x = (x / iter_frame.size.x) * (config.z_size.x) + (config.z_min.x);
//			c.x = (x / surface.size.x) * (config.z_max.x - config.z_min.x) + (config.z_min.x);
			depth = config.fractal_fn(c, config.depth_max, config.z_mouse);
			depth |= chunk_mask << 15;
			iter_frame.iter[(int)(y * iter_frame.size.x + x)] = (uint16_t)depth;
			x++;
		}
		y++;
	}
}

int	get_mandelbrot_value(t_float2 c, int depth_max)
{
	t_float2	z;
	int			depth;
	float		z_y_temp;

	depth = 1;
	z = c;
	while (((z.x * z.x) + (z.y * z.y) < 4) && (depth < depth_max))
	{
		z_y_temp = (2 * z.x * z.y) + c.y;
		z.x = (z.x * z.x) - (z.y * z.y) + c.x;
		z.y = z_y_temp;
		depth++;
	}
	if (depth >= depth_max)
		return (0);
	return (depth);
}

int	get_burningship_value(t_float2 c, int depth_max)
{
	t_float2	z;
	int			depth;
	float		z_y_temp;

	depth = 1;
	z = c;
	while (((z.x * z.x) + (z.y * z.y) < 4) && (depth < depth_max))
	{
		z_y_temp = 2 * z.x * z.y;
		z.x = (z.x * z.x) - (z.y * z.y) + c.x;
		z.y = fabsf(z_y_temp) + c.y;
		z.x = fabsf(z.x);
		depth++;
	}
	if (depth >= depth_max)
		return (0);
	return (depth);
}

int	get_julia_value(t_float2 z_in, int depth_max, t_float2 c)
{
	t_float2	z;
	int			depth;
	float		z_y_temp;

	depth = 1;
	z = z_in;
	while (((z.x * z.x) + (z.y * z.y) < 4) && (depth < depth_max))
	{
		z_y_temp = (2 * z.x * z.y) + c.y;
		z.x = (z.x * z.x) - (z.y * z.y) + c.x;
		z.y = z_y_temp;
		depth++;
	}
	if (depth >= depth_max)
		return (0);
	return (depth);
}