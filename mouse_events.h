#ifndef FRACTOL_MOUSE_EVENTS_H
#define FRACTOL_MOUSE_EVENTS_H

int	mouse_move(int x, int y, void *param);
int	mouse_down(int button, int x, int y, void *param);
int	mouse_up(int button, int x, int y, void *param);

#endif
