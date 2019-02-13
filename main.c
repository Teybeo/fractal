#include <stdio.h>
#include "app.h"

int	main()
{
	t_app app;
	setbuf(stdout, NULL);
	app_init(&app);
	app_run(&app);
	return (0);
}

