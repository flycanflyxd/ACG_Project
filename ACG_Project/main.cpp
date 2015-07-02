#include "draw.h"
#include <iostream>
#include <ctime>

using namespace std;

int main()
{
	Camera camera;
	Viewport viewport;
	vector<Light> lights;
	vector<Sphere> spheres;
	vector<Triangle> triangles;
	vector<Plane> planes;
	if (!init(camera, viewport, lights, spheres, triangles, planes))
	{
		cerr << "Cannot read input file" << endl;
		system("pause");
		return 1;
	}
	rayTracing(camera, viewport, lights, spheres, triangles, planes);
	output(viewport);
	cout << "Time used: " << static_cast<double>(clock()) / CLK_TCK << endl;
	system("pause");
	return 0;
}