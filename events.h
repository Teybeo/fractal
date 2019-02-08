#ifndef EVENTS_H
# define EVENTS_H

int	keydown_event(int keycode, void *param);
int	keyup_event(int keycode, void *param);
int	mouse_down(int button, int x, int y, void *param);
int	mouse_up(int button, int x, int y, void *param);
int	quit_event();

#endif
