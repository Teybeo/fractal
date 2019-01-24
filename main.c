#include <stdio.h>
#include "app.h"

int	main()
{
	t_app app;
	setbuf(stdout, NULL);
	app_init(&app);
	return (0);
}

