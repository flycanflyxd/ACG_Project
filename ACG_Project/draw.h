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

bool init(Camera &camera, Viewport &viewport, Light &light, std::vector<Sphere> &spheres, std::vector<Triangle> &triangles, std::vector<Plane> &planes);
Intersection PhongShading(vec3 startPosition, Intersection intersection, Light light);
Intersection shadow(Intersection point, Light light, std::vector<Sphere> &spheres, std::vector<Triangle> &triangles, std::vector<Plane> &planes);
Intersection rayTracer(vec3 startPosition, vec3 ray, std::vector<Sphere> &spheres, std::vector<Triangle> &triangles, std::vector<Plane> &planes);
void rayTracing(Camera &camera, Viewport &viewport, Light light, std::vector<Sphere> &spheres, std::vector<Triangle> &triangles, std::vector<Plane> &planes);
vec3 draw(Camera &camera, vec3 ray, Light light, std::vector<Sphere> &spheres, std::vector<Triangle> &triangles, std::vector<Plane> &planes);
void output(Viewport &viewport);

#endif