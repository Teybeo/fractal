#include "t_double2.h"

t_double2	double2_sub(t_double2 a, t_double2 b)
{
	return (t_double2){
			a.x - b.x,
			a.y - b.y};
}

t_double2	double2_add(t_double2 a, t_double2 b)
{
	return (t_double2){
			a.x + b.x,
			a.y + b.y};
}

void		double2_add_this(t_double2 *this, t_double2 b)
{
	this->x += b.x;
	this->y += b.y;
}

void		double2_sub_this(t_double2 *this, t_double2 b)
{
	this->x -= b.x;
	this->y -= b.y;
}

void		double2_mul_this(t_double2 *this, double x)
{
	this->x *= x;
	this->y *= x;
}

/*
* The source space origin is ommited as it is assumed to be (0, 0)
*/
void	double2_remap(t_double2 *this, t_double2 src_size,
		t_double2 dst_size, t_double2 dst_origin)
{
	this->x = dst_origin.x + (this->x / src_size.x) * dst_size.x;
	this->y = dst_origin.y + (this->y / src_size.y) * dst_size.y;
}

t_double2	get_center(t_double2 min, t_double2 max)
{
	return (t_double2){
			(max.x - min.x) / 2.f + min.x,
			(max.y - min.y) / 2.f + min.y
	};
}

