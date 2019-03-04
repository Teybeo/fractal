/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/23 18:11:09 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/03/04 18:24:17 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void	print_usage(void)
{
	puts("Usage:");
	puts("  ./fractol");
	puts("  ./fractol mandelbrot");
	puts("  ./fractol julia");
	puts("  ./fractol burning_ship");
}

int		parse_fractal_index(int argc, char **argv)
{
	int		fractal_index;

	if (argc != 2)
	{
		print_usage();
		exit(0);
	}
	fractal_index = -1;
	if (strcmp(argv[1], "mandelbrot") == 0)
		fractal_index = 0;
	if (strcmp(argv[1], "julia") == 0)
		fractal_index = 1;
	if (strcmp(argv[1], "burning_ship") == 0)
		fractal_index = 2;
	if (fractal_index == -1)
	{
		print_usage();
		exit(0);
	}
	return (fractal_index);
}

int		main(int argc, char **argv)
{
	t_app	app;
	int		fractal_index;

	fractal_index = parse_fractal_index(argc, argv);
	app_init(&app, fractal_index);
	app_run(&app);
	return (0);
}
