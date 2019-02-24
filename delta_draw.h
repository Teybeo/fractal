/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delta_draw.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/23 17:00:38 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/02/23 21:01:39 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DELTA_DRAW_H
# define DELTA_DRAW_H

# include "t_double2.h"
# include "multithreading.h"
# include "coloring.h"

void	delta_draw(t_double2 delta, t_config *config, t_surface16 iter_frame,
		t_surface color_frame, t_thread_pool *pool);
void	copy_region(t_double2 src, t_double2 dst, t_double2 region_size,
		t_surface16 surface, t_surface color_frame);
t_rect	get_wide_dirty_rect(t_double2 frame_size, t_double2 delta);
t_rect	get_tall_dirty_rect(t_double2 frame_size, t_double2 delta);

#endif
