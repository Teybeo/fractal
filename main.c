/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/23 18:11:09 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/03/05 18:38:08 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app.h"
#include "libft/libft.h"
#include <unistd.h>
#include <stdlib.h>

void	print_usage(void)
{
	ft_puts("Usage:");
	ft_puts("  ./fractol");
	ft_puts("  ./fractol mandelbrot");
	ft_puts("  ./fractol julia");
	ft_puts("  ./fractol burning_ship");
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
	if (ft_strequ(argv[1], "mandelbrot"))
		fractal_index = 0;
	if (ft_strequ(argv[1], "julia"))
		fractal_index = 1;
	if (ft_strequ(argv[1], "burning_ship"))
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
