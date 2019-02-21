#include "delta_draw.h"

#include <math.h>
#include <stdio.h>
#include "config.h"
#include "multithreading.h"
#include "coloring.h"

void delta_draw(t_double2 delta, t_config *config, t_surface16 iter_frame, t_surface color_frame, t_thread_pool *pool)
{
	t_double2	src;
	t_double2	dst;
	t_double2	region_size;
	t_rect		tall_dirty_rect;
	t_rect		wide_dirty_rect;

	config_move_by_delta(config, delta, iter_frame.size);
	src.x = (delta.x >= 0) ? 0 : -delta.x;
	src.y = (delta.y >= 0) ? 0 : -delta.y;
	dst.x = (delta.x >= 0) ? delta.x : 0;
	dst.y = (delta.y >= 0) ? delta.y : 0;
	region_size.x = iter_frame.size.x - fabs(delta.x);
	region_size.y = iter_frame.size.y - fabs(delta.y);
	copy_region(src, dst, region_size, iter_frame, color_frame);
//	draw_iter_region(*config, (t_rect){{}, iter_frame.size}, iter_frame);
//	draw_color(color_frame, iter_frame, *config);
//	return;
	wide_dirty_rect = get_wide_dirty_rect(iter_frame.size, delta);
	tall_dirty_rect = get_tall_dirty_rect(iter_frame.size, delta);
	draw_iter_region_parallel_pool(*config, pool, iter_frame, wide_dirty_rect);
	draw_iter_region_parallel_pool(*config, pool, iter_frame, tall_dirty_rect);
	draw_color_region(*config, wide_dirty_rect, color_frame, iter_frame);
	draw_color_region(*config, tall_dirty_rect, color_frame, iter_frame);
}

t_rect	get_wide_dirty_rect(t_double2 frame_size, t_double2 delta)
{
	t_rect	rect;

	rect.size.x = frame_size.x;
	rect.size.y = fabs(delta.y);
	rect.origin.x = 0;
	rect.origin.y = (delta.y > 0) ? 0 : (frame_size.y - fabs(delta.y));
	return (rect);
}

t_rect	get_tall_dirty_rect(t_double2 frame_size, t_double2 delta)
{

	t_rect	rect;

	rect.size.x = fabs(delta.x);
	rect.size.y = (frame_size.y - fabs(delta.y));
	rect.origin.x = (delta.x > 0) ? 0 : (frame_size.x - fabs(delta.x));
	rect.origin.y = (delta.y > 0) ? delta.y : 0;
	return (rect);
}

void	copy_region(t_double2 src, t_double2 dst, t_double2 region_size, t_surface16 surface, t_surface color_frame)
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
