#ifndef FRACTOL_CONFIG_H
#define FRACTOL_CONFIG_H

#include <stdbool.h>
#include <stdint.h>
#include "t_double2.h"

#define DEPTH_MAX 1000
#define LINES_PER_CHUNK 5

typedef int	(*fractal_fn)();

struct	s_config
{
	int			depth_max;
	t_double2	z_min;
	t_double2	z_max;
	t_double2	z_size;
	t_double2	z_mouse;
	short		lines_per_chunk;
	fractal_fn	fractal_fn;
	bool		show_chunks;
	uint32_t	*palette;
};
typedef struct s_config	t_config;

struct s_rect {
	t_double2 origin;
	t_double2 size;
};
typedef struct s_rect	t_rect;

#define ZOOM (0.5)
#define DEZOOM (2)

t_config	config_init(t_double2 win_size);
void		config_zoom_to(t_config *config, int x, int y, t_double2 win_size);
void		config_dezoom_from(t_config *config, int x, int y, t_double2 win_size);
void		config_zoom_factor(t_config *config, double factor);
void		config_move_by_delta(t_config *config, t_double2 delta, t_double2 win_size);

#endif
