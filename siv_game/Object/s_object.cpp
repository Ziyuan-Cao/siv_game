#include "s_object.h"

template<>
void s_object<Box>::update_state()
{
	mesh.setPos(position[0], position[1], position[2]);
	mesh.setSize(scale, scale, scale);
}

template<>
void s_object<Sphere>::update_state()
{
	mesh.setPos(position[0], position[1], position[2]);
	mesh.setR(scale);
}

template<>
void s_object<Plane>::update_state()
{
	mesh.setPos(position[0], position[1], position[2]);
	mesh.setSize(scale, scale);
	
}
