#include "coloring.h"

void app_render_colors(t_surface surface, t_surface16 iter_frame, t_config config)
{
	int i;

	i = 0;
	while (i < (iter_frame.size.x * iter_frame.size.y))
	{
		int depth = iter_frame.iter[i];
		int color = (255.f * (depth / (float)config.depth_max));
		surface.pixels[i] = color << 8;
		i++;
	}
}
