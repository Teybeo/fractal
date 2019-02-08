#include <math.h>
#include "config.h"
#include "drawing.h"
#include "coloring.h"

t_rect	get_wide_dirty_rect(t_float2 frame_size, t_rect skip_rect, t_float2 delta)
{
	t_rect rect;
	rect.size.x = frame_size.x;
	rect.size.y = fabsf(delta.y);
	rect.origin.x = 0;
	rect.origin.y = (delta.y > 0) ? skip_rect.size.y : 0;
	return rect;
}

t_rect	get_tall_dirty_rect(t_float2 frame_size, t_rect skip_rect, t_float2 delta)
{
	t_rect rect;
	rect.size.x = fabsf(delta.x);
	rect.size.y = skip_rect.size.y;
	rect.origin.x = (delta.x > 0) ? skip_rect.size.x : 0;
	rect.origin.y = (delta.y > 0) ? 0 : skip_rect.origin.y;
	(void)frame_size;
	return rect;
}

void	copy_region(t_float2 src, t_float2 dst, t_float2 region_size, t_surface16 surface, t_surface color_frame)
{
	if ((dst.y < src.y) || ((dst.y == src.y) && (dst.x < src.x)))
	{
		for (int y = 0; y < region_size.y; ++y)
		{
			for (int x = 0; x < region_size.x; ++x)
			{
				int src_index = (int) ((src.y + y) * surface.size.x + src.x + x);
				int dst_index = (int) ((dst.y + y) * surface.size.x + dst.x + x);
				surface.iter[dst_index] = surface.iter[src_index];
				color_frame.pixels[dst_index] = color_frame.pixels[src_index];
			}
		}
	}
	else
	{
		for (int y = region_size.y - 1; y >= 0; --y)
		{
			for (int x = region_size.x - 1; x >= 0; --x)
			{
				int src_index = (int) ((src.y + y) * surface.size.x + src.x + x);
				int dst_index = (int) ((dst.y + y) * surface.size.x + dst.x + x);
				surface.iter[dst_index] = surface.iter[src_index];
				color_frame.pixels[dst_index] = color_frame.pixels[src_index];
			}
		}
	}
}
