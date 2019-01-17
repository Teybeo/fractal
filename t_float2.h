#ifndef T_FLOAT2_H
# define T_FLOAT2_H

struct	s_float2 {
	float x;
	float y;
};
typedef struct s_float2	t_float2;

t_float2	float2_sub(t_float2 a, t_float2 b);
t_float2	float2_add(t_float2 a, t_float2 b);

void		float2_add_this(t_float2 *this, t_float2 b);
void		float2_sub_this(t_float2 *this, t_float2 b);
void		float2_mul_this(t_float2 *this, float x);
void		float2_remap(t_float2 *this, t_float2 src_size,
					 t_float2 dst_size, t_float2 dst_origin);
t_float2	get_center(t_float2 min, t_float2 max);

#endif
