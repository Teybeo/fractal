/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/23 18:11:09 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/03/06 18:31:17 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app.h"
#include "libft/libft.h"
#include "fractal_list.h"
#include <stdlib.h>

void	print_usage(void)
{
	ft_puts("Usage:");
	ft_puts("  ./fractol");
	ft_puts("  ./fractol mandelbrot");
	ft_puts("  ./fractol julia");
	ft_puts("  ./fractol burning_ship");
}

int		main(int argc, char **argv)
{
	t_app	app;
	int		fractal_index;

	if (argc != 2)
	{
		print_usage();
		exit(0);
	}
	fractal_index = get_fractal_index_by_name(argv[1]);
	if (fractal_index == -1)
	{
		print_usage();
		exit(0);
	}
	app_init(&app, fractal_index);
	app_run(&app);
	return (0);
}
