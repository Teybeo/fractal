/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractal_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/06 18:33:39 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/03/11 14:03:38 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <assert.h>
#include "drawing.h"
#include "fractal_list.h"
#include "fractal_functions.h"
#include "libft/libft.h"
#include "fractal_functions.h"

t_fractal	*get_fractal_list(void)
{
	static t_fractal fractal_list[FRACTAL_COUNT] = {
			(t_fractal){get_mandelbrot_value, "mandelbrot", false},
			(t_fractal){get_mandelbrot3_value, "mandelbrot3", false},
			(t_fractal){get_burningship_value, "burningship", false},
			(t_fractal){get_burningmandelbrot3_value, "absmandelbrot3", false},
			(t_fractal){get_julia_value, "julia", true},
			(t_fractal){get_julia3_value, "julia3", true},
			(t_fractal){get_burning_julia_value, "absjulia", true},
			(t_fractal){get_burningjulia3_value, "absjulia3", true},
	};

	return (fractal_list);
}

t_fractl_fn	get_fractal_fn(int index)
{
	t_fractal	*fractal_list;

	assert(index >= 0 && index < FRACTAL_COUNT);
	fractal_list = get_fractal_list();
	return (fractal_list[index].fn_ptr);
}

const char	*get_fractal_name(int index)
{
	t_fractal	*fractal_list;

	assert(index >= 0 && index < FRACTAL_COUNT);
	fractal_list = get_fractal_list();
	return (fractal_list[index].name);
}

char		get_fractal_index_by_name(const char *name)
{
	char		i;
	t_fractal	*fractal_list;

	i = 0;
	fractal_list = get_fractal_list();
	while (i < FRACTAL_COUNT)
	{
		if (ft_strequ(name, fractal_list[(int)i].name))
			return (i);
		i++;
	}
	return (-1);
}

bool		is_julia_type(int index)
{
	t_fractal	*fractal_list;

	assert(index >= 0 && index < FRACTAL_COUNT);
	fractal_list = get_fractal_list();
	return (fractal_list[index].is_julia_type);
}
