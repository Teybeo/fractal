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
	int			i;
	t_fractal	*fractal_list;

	ft_puts("Usage:");
	ft_puts("  ./fractol");
	i = 0;
	fractal_list = get_fractal_list();
	while (i < FRACTAL_COUNT)
	{
		ft_putstr("  ./fractol ");
		ft_puts(fractal_list[i].name);
		i++;
	}
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
