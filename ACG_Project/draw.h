#ifndef _DRAW_H
#define _DRAW_H

#include <string>
#include <fstream>
#include <vector>
#include "geometry.h"
#include "camera.h"
#include "viewport.h"
#include "light.h"
#include "imageIO.h"
#include "material.h"

bool init(Camera &camera, Viewport &viewport, std::vector<Light> &lights, std::vector<Sphere> &spheres, std::vector<Triangle> &triangles, std::vector<Plane> &planes);
void rayTracing(Camera &camera, Viewport &viewport, std::vector<Light> lights, std::vector<Sphere> &spheres, std::vector<Triangle> &triangles, std::vector<Plane> &planes);
void output(Viewport &viewport);

#endif