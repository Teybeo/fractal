/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/23 17:02:04 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/02/23 21:01:39 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_H
# define CONFIG_H

# include <stdbool.h>
# include <stdint.h>
# include "t_double2.h"

# define DEPTH_MAX 1000
# define LINES_PER_CHUNK 5

typedef int	(*t_fractl_fn)();

struct			s_config
{
	int			depth_max;
	t_double2	z_min;
	t_double2	z_max;
	t_double2	z_size;
	t_double2	z_mouse;
	short		lines_per_chunk;
	t_fractl_fn	fractal_fn;
	bool		show_chunks;
	bool		show_palette;
	uint32_t	*palette;
};
typedef struct s_config	t_config;

struct			s_rect {
	t_double2	origin;
	t_double2	size;
};
typedef struct s_rect	t_rect;

# define ZOOM (0.5)
# define DEZOOM (2)

t_config		config_init(t_double2 win_size);
void			config_zoom_factor(t_config *cfg, double factor);
void			config_zoom_to(t_config *cfg, int x, int y,
									t_double2 win_size);

void			config_dezoom_from(t_config *cfg, int x, int y,
									t_double2 win_size);

void			config_move_by_delta(t_config *cfg, t_double2 delta,
									t_double2 win_size);

#endif
