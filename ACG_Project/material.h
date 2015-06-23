#ifndef _MATERIAL_H
#define _MATERIAL_H

#include "algebra3.h"

class Material
{
public:
	vec3 color;
	float solidAngle[2];
	void setMaterial(vec3 color, float solidAngle[])
	{
		this->color = color;
		for (int i = 0; i < 2; i++)
		{
			this->solidAngle[i] = solidAngle[i];
		}
	}
};

#endif