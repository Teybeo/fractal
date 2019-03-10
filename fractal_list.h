/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractal_list.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/06 18:33:34 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/03/06 19:39:54 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_FRACTAL_LIST_H
# define FRACTOL_FRACTAL_LIST_H

# define FRACTAL_COUNT 8

typedef struct	s_fractal {
	t_fractl_fn	fn_ptr;
	char		*name;
	bool		is_julia_type;
}				t_fractal;


t_fractal		*get_fractal_list(void);
t_fractl_fn		get_fractal_fn(int index);
const char		*get_fractal_name(int index);
char			get_fractal_index_by_name(const char *name);
bool			is_julia_type(int index);

#endif
