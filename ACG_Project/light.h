#ifndef _LIGHT_H
#define _LIGHT_H

#include "algebra3.h"
#include "geometry.h"

class Light: public Sphere
{
public:
	void setLight(vec3 center, float radius, Material material)
	{
		this->center = center;
		this->radius = radius;
		this->material = material;
	}
};

#endif