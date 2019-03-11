/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractal_functions.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/06 15:28:42 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/03/11 14:03:15 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_FRACTAL_FUNCTIONS_H
# define FRACTOL_FRACTAL_FUNCTIONS_H

# include "t_double2.h"

int			get_mandelbrot_value(t_double2 c, int depth_max);
int			get_mandelbrot3_value(t_double2 c, int depth_max);
int			get_burningship_value(t_double2 c, int depth_max);
int			get_burningmandelbrot3_value(t_double2 c, int depth_max);

int			get_julia_value(t_double2 c, int depth_max, t_double2 z_in);
int			get_julia3_value(t_double2 z_in, int depth_max, t_double2 c);
int			get_burning_julia_value(t_double2 z_in, int depth_max, t_double2 c);
int			get_burningjulia3_value(t_double2 z_in, int depth_max, t_double2 c);

#endif
