/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multithreading.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/23 16:51:20 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/02/27 20:48:02 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MULTITHREADING_H
# define MULTITHREADING_H

# include <stdint.h>
# include "t_double2.h"
# include "config.h"
# include "thread_pool.h"
# include "drawing.h"

# define MAX_TASK_COUNT 2048

typedef struct	s_thread_config {
	t_config	config;
	t_double2	win_size;
	uint16_t	*pixels;
	t_rect		rect;
	short		thread_id;
}				t_task_data;

void			compute_region_parallel(t_config cfg, t_thread_pool *pool,
					t_surface16 iter_frame, t_rect rect);
int				get_chunk_count(int line_count, int lines_per_chunk);
void			prepare_task_chunks(t_task_data *task_data, int chunk_count,
					int chunk_height, t_rect rect);

#endif
