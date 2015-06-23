#include "draw.h"
#include <iostream>
#include <ctime>

using namespace std;

int main()
{
	Camera camera;
	Viewport viewport;
	Light light;
	vector<Sphere> spheres;
	vector<Triangle> triangles;
	vector<Plane> planes;
	if (!init(camera, viewport, light, spheres, triangles, planes))
	{
		cerr << "Cannot read input file" << endl;
		system("pause");
		return 1;
	}
	rayTracing(camera, viewport, light, spheres, triangles, planes);
	output(viewport);
	cout << "Time used: " << static_cast<double>(clock()) / CLK_TCK << endl;
	system("pause");
	return 0;
}