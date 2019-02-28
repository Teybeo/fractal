/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delta_draw.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/23 20:24:43 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/02/28 14:21:43 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "delta_draw.h"

#include <math.h>
#include <stdio.h>
#include <string.h>
#include "config.h"
#include "multithreading.h"
#include "coloring.h"

void	delta_draw(t_double2 delta, t_config *config, t_frame frame,
		t_thread_pool *pool)
{
	t_double2	src;
	t_double2	dst;
	t_double2	region_size;
	t_rect		tall_dirty_rect;
	t_rect		wide_dirty_rect;

	config_move_by(config, delta, frame.size);
	src.x = (delta.x >= 0) ? 0 : -delta.x;
	src.y = (delta.y >= 0) ? 0 : -delta.y;
	dst.x = (delta.x >= 0) ? delta.x : 0;
	dst.y = (delta.y >= 0) ? delta.y : 0;
	region_size.x = frame.size.x - fabs(delta.x);
	region_size.y = frame.size.y - fabs(delta.y);
	copy_region(src, dst, region_size, frame);
	wide_dirty_rect = get_wide_dirty_rect(frame.size, delta);
	tall_dirty_rect = get_tall_dirty_rect(frame.size, delta);
	compute_region_parallel(*config, pool, frame, wide_dirty_rect);
	compute_region_parallel(*config, pool, frame, tall_dirty_rect);
	draw_color_region(*config, frame, wide_dirty_rect);
	draw_color_region(*config, frame, tall_dirty_rect);
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

/*
** Basically a 2D memmove
*/

void	copy_region(t_double2 src, t_double2 dst, t_double2 region_size,
		t_frame frame)
{
	int		src_idx;
	int		dst_idx;
	int		stride;
	size_t	pxl_line_bytes;
	size_t	iter_line_bytes;

	stride = (int)frame.size.x;
	src_idx = (int)((src.y * stride) + src.x);
	dst_idx = (int)((dst.y * stride) + dst.x);
	if (dst_idx > src_idx)
	{
		src_idx += stride * ((int)region_size.y - 1);
		dst_idx += stride * ((int)region_size.y - 1);
		stride *= -1;
	}
	pxl_line_bytes = (size_t)(region_size.x * sizeof(*frame.pixels));
	iter_line_bytes = (size_t)(region_size.x * sizeof(*frame.iter));
	while (region_size.y--)
	{
		memmove(frame.iter + dst_idx, frame.iter + src_idx, iter_line_bytes);
		memmove(frame.pixels + dst_idx, frame.pixels + src_idx, pxl_line_bytes);
		dst_idx += stride;
		src_idx += stride;
	}
}

/*
** If delta is bigger than frame, the rect target for delta goes out of bounds
** So we have to fallback to a full frame recompute
** Even if delta is < frame size, it can be better to just do a single redraw
** Instead of copy_region + 2 smaller recompute (with a barrier in between)
*/

void	try_delta_draw(t_double2 delta, t_config *config, t_frame frame,
		t_thread_pool *pool)
{
	t_rect	rect;

	if (fabs(delta.x) > frame.size.x * 0.75
		|| fabs(delta.y) > frame.size.y * 0.75)
	{
		rect = (t_rect) {{0, 0}, frame.size};
		config_move_by(config, delta, frame.size);
		compute_region_parallel(*config, pool, frame, rect);
		draw_color(*config, frame);
	}
	else
		delta_draw(delta, config, frame, pool);
}
