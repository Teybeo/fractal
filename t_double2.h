/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_double2.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/23 16:21:32 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/02/23 21:01:39 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef T_DOUBLE2_H
# define T_DOUBLE2_H

struct		s_double2 {
	double	x;
	double	y;
};
typedef struct s_double2	t_double2;

t_double2	double2_sub(t_double2 a, t_double2 b);
t_double2	double2_add(t_double2 a, t_double2 b);

void		double2_add_this(t_double2 *this, t_double2 b);
void		double2_sub_this(t_double2 *this, t_double2 b);
void		double2_mul_this(t_double2 *this, double x);

void		double2_remap(t_double2 *this, t_double2 src_size,
						t_double2 dst_size, t_double2 dst_origin);
t_double2	get_center(t_double2 min, t_double2 max);

#endif
