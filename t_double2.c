#include "t_double2.h"

t_float2	float2_sub(t_float2 a, t_float2 b)
{
	return (t_float2){
			a.x - b.x,
			a.y - b.y};
}

t_float2	float2_add(t_float2 a, t_float2 b)
{
	return (t_float2){
			a.x + b.x,
			a.y + b.y};
}

void		float2_add_this(t_float2 *this, t_float2 b)
{
	this->x += b.x;
	this->y += b.y;
}

void		float2_sub_this(t_float2 *this, t_float2 b)
{
	this->x -= b.x;
	this->y -= b.y;
}

void		float2_mul_this(t_float2 *this, float x)
{
	this->x *= x;
	this->y *= x;
}

/*
* The source space origin is ommited as it is assumed to be (0, 0)
*/
void	float2_remap(t_float2 *this, t_float2 src_size,
		t_float2 dst_size, t_float2 dst_origin)
{
	this->x = dst_origin.x + (this->x / src_size.x) * dst_size.x;
	this->y = dst_origin.y + (this->y / src_size.y) * dst_size.y;
}

t_float2	get_center(t_float2 min, t_float2 max)
{
	return (t_float2){
			(max.x - min.x) / 2.f + min.x,
			(max.y - min.y) / 2.f + min.y
	};
}

