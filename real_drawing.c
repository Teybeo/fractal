#include "real_drawing.h"

#include <math.h>

#if 1
int	get_mandelbrot_value(t_float2 c, int depth_max)
{
	t_float2	z;
	int			depth;

	depth = 0;
	z = c;
	while (((z.x * z.x) + (z.y * z.y) < 4) && (depth < depth_max))
	{
		float z_y_temp = 2 * z.x * z.y + c.y;
		z.x = (z.x * z.x) - (z.y * z.y) + c.x;
		z.y = z_y_temp;
		depth++;
	}
	if (depth >= depth_max)
		return (0);
	return (depth);
}
#else
int	get_mandelbrot_value(t_float2 c, int depth_max)
{
	t_float2	z;
	t_float2	z_squared;
	int			depth;

	depth = 0;
	z = (t_float2){};
	z_squared = (t_float2){z.x * z.x, z.y * z.y};
	while (((z_squared.x + z_squared.y) < 4) && (depth < depth_max))
	{
		z.y = 2 * z.x * z.y + c.y;
		z.x = z_squared.x - z_squared.y + c.x;
		depth++;
		z_squared = (t_float2){z.x * z.x, z.y * z.y};
	}
	if (depth == depth_max)
		return (0);
	return (depth - 1);
}
#endif

int	get_burningship_value(t_float2 c, int depth_max)
{
	t_float2	z;
	int			depth;
	float		z_y_temp;

	depth = 0;
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
