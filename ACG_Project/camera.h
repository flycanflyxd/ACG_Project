#ifndef _CAMERA_H
#define _CAMERA_H

class Camera
{
public:
	const static int CAMERA_NUM = 37;
	vec3 position[CAMERA_NUM];
	vec3 direction;
	float verticalFOV, horizontalFOV;
	float F = 1.8;
	float aperture = F / 3.5;
};

#endif