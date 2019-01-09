/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/11 15:08:17 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/01/09 14:38:44 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef APP_H
# define APP_H

# include "t_float2.h"

# include <stdint.h>
# include <stdbool.h>

enum	e_key
{
	KEY_ESCAPE = 53,
	KEY_UP = 13,
	KEY_DOWN = 1,
	KEY_RIGHT = 2,
	KEY_LEFT = 0,
	KEY_R = 15,
	KEY_MINUS = 7,
	KEY_PLUS = 8,
	KEY_ZOOM = 12,
	KEY_DEZOOM = 6,
	KEY_LESS = 78,
	KEY_MORE = 69,
	KEY_LAST = 512
};

struct	s_config
{
	int			depth_max;
	t_float2	z_min;
	t_float2	z_max;
	t_float2	z_size;
};
typedef struct s_config	t_config;

typedef struct s_rect {
	t_float2 origin;
	t_float2 size;
} t_rect;

struct	s_app
{
	void		*mlx_context;
	void		*mlx_texture;
	void		*mlx_window;
	t_float2	win_size;
	t_rect		skip_rect;
	uint32_t	*pixels;
	t_config	config;
	int			thread_count;
	bool		keystate[KEY_LAST];
	bool		is_dragging;
	bool		need_full_redraw;
};
typedef struct s_app	t_app;

void	app_init(t_app *app);
void	app_draw(t_app app);
t_app	app_run();
int		app_callback(void *param);

#endif
