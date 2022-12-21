#include "s_math.h"

double distance_cal(double p1[3], double p2[3])
{
	return sqrt(
		(p1[0] - p2[0]) * (p1[0] - p2[0])+
		(p1[1] - p2[1]) * (p1[1] - p2[1]) +
		(p1[2] - p2[2]) * (p1[2] - p2[2]));
}

//p1->p2
void direction_cal(double p1[3], double p2[3], double& out_x, double& out_y, double& out_z)
{
	out_x = p2[0] - p1[0];
	out_y = p2[1] - p1[1];
	out_z = p2[2] - p1[2];
	double v_1[3] = { out_x ,out_y,out_z };
	double zero_v[3] = { 0 };
	double vector_mod = distance_cal(v_1, zero_v);
	out_x /= vector_mod;
	out_y /= vector_mod;
	out_z /= vector_mod;
}
