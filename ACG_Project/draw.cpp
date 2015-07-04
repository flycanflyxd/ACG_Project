#include "draw.h"
#include <iostream>
#include <cmath>
#include <ctime>

using namespace std;

bool init(Camera &camera, Viewport &viewport, vector<Light> &lights, vector<Sphere> &spheres, vector<Triangle> &triangles, vector<Plane> &planes)
{
	char type;
	float input[10];
	string trash;
	Triangle triangle;
	Plane plane;
	Material material;
	Light light;
	vec3 position;
	float radius;
	srand(time(NULL));

	ifstream fin("input.txt");
	if (!fin)
		return false;
	while (!fin.eof())
	{
		fin >> type;
		switch (type)
		{
		case 'E':
			fin >> camera.position[0] >> camera.position[1] >> camera.position[2];
			break;
		case 'V':
			fin >> camera.direction[0] >> camera.direction[1] >> camera.direction[2];
			camera.direction.normalize();
			break;
		case 'F':
			fin >> camera.FOV;
			break;
		case 'R':
			fin >> viewport.width;
			fin >> viewport.height;
			viewport.pixel = new vec3*[viewport.height];
			for (int i = 0; i < viewport.height; i++)
				viewport.pixel[i] = new vec3[viewport.width];
			break;
		case 'S':
			for (int i = 0; i < 4; i++)
				fin >> input[i];
			spheres.push_back(Sphere(input[0], input[1], input[2], input[3], material));
			break;
		case 'T':
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
					fin >> input[j];
				triangle.vertices[i].set(input[0], input[1], input[2]);
			}
			triangle.material = material;
			triangles.push_back(triangle);
			break;
		case 'P':
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 3; j++)
					fin >> input[j];
				plane.vertices[i].set(input[0], input[1], input[2]);
			}
			plane.material = material;
			planes.push_back(plane);
			break;
		case 'L':
			for (int i = 0; i < 3; i++)
			{
				fin >> position[i];
			}
			fin >> radius;
			for (int i = 0; i < 3; i++)
			{
				fin >> material.color[i];
			}
			material.color /= 255;
			for (int i = 0; i < 2; i++)
			{
				fin >> material.solidAngle[i];
			}
			light.setLight(position, radius, material);
			lights.push_back(light);
			break;
		case 'M':
			for (int i = 0; i < 3; i++)
			{
				fin >> material.color[i];
			}
			material.color /= 255;
			for (int i = 0; i < 2; i++)
			{
				fin >> material.solidAngle[i];
			}
			break;
		default:
			//return false;
			getline(fin, trash);
			break;
		}
	}
	return true;
}


Intersection rayTracer(vec3 startPosition, vec3 ray, vector<Light> lights, vector<Sphere> &spheres, vector<Triangle> &triangles, vector<Plane> &planes)
{
	vec3 v1, v2, normal;
	vec3 intersectPosition;
	Intersection intersection;
	float a = 0, b = 0, c = 0, t = 0, d = 0;
	//light
	for (int nLight = 0; nLight < lights.size(); nLight++)
	{
		a = 1; //a = nx^2 + ny^2 + nz^2 = 1 cuz it's normalized
		for (int k = 0; k < 3; k++)
		{
			b += 2 * ray[k] * (startPosition[k] - lights[nLight].center[k]);
			c += pow(startPosition[k] - lights[nLight].center[k], 2);
		}
		c -= pow(lights[nLight].radius, 2);
		if (pow(b, 2) - 4 * a * c >= 0)
		{
			t = (-b - sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
			if (t < intersection.t && t > 0)
			{
				intersection.type = 'l';
				intersection.index = nLight;
				intersection.t = t;
				intersection.position = startPosition + ray * t;
				intersection.normal = (intersection.position - lights[nLight].center).normalize();
				intersection.material = lights[nLight].material;
			}
		}
		a = b = c = 0;
	}
	// Sphere
	for (int nSphere = 0; nSphere < spheres.size(); nSphere++)
	{
		a = 1; //a = nx^2 + ny^2 + nz^2 = 1 cuz it's normalized
		for (int k = 0; k < 3; k++)
		{
			b += 2 * ray[k] * (startPosition[k] - spheres[nSphere].center[k]);
			c += pow(startPosition[k] - spheres[nSphere].center[k], 2);
		}
		c -= pow(spheres[nSphere].radius, 2);
		if (pow(b, 2) - 4 * a * c >= 0)
		{
			t = (-b - sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
			if (t < intersection.t && t > 0)
			{
				intersection.type = 's';
				intersection.index = nSphere;
				intersection.t = t;
				intersection.position = startPosition + ray * t;
				intersection.normal = (intersection.position - spheres[nSphere].center).normalize();
				intersection.material = spheres[nSphere].material;
			}
		}
		a = b = c = 0;
	}
	// Triangle
	for (int nTriangle = 0; nTriangle < triangles.size(); nTriangle++)
	{
		v1 = triangles[nTriangle].vertices[1] - triangles[nTriangle].vertices[0];
		v2 = triangles[nTriangle].vertices[2] - triangles[nTriangle].vertices[0];
		normal = (v1 ^ v2).normalize();
		d = normal[0] * triangles[nTriangle].vertices[0][0] + normal[1] * triangles[nTriangle].vertices[0][1] + normal[2] * triangles[nTriangle].vertices[0][2];
		t = (d - (normal[0] * startPosition[0] + normal[1] * startPosition[1] + normal[2] * startPosition[2]))
			/ (normal[0] * ray[0] + normal[1] * ray[1] + normal[2] * ray[2]);
		intersectPosition = startPosition + ray * t;
		if (((triangles[nTriangle].vertices[1] - triangles[nTriangle].vertices[0]) ^ (intersectPosition - triangles[nTriangle].vertices[0])) * normal >= 0
			&& ((triangles[nTriangle].vertices[2] - triangles[nTriangle].vertices[1]) ^ (intersectPosition - triangles[nTriangle].vertices[1])) * normal >= 0
			&& ((triangles[nTriangle].vertices[0] - triangles[nTriangle].vertices[2]) ^ (intersectPosition - triangles[nTriangle].vertices[2])) * normal >= 0)
		{
			/*
			(p1- p0) x (intersection - p0) * normal >= 0
			&& (p2- p1) x (intersection - p1) * normal >= 0
			&& (p0- p2) x (intersection - p2) * normal >= 0
			the intersection is at the same side of each line
			*/
			if (t < intersection.t && t > 0)
			{
				intersection.type = 't';
				intersection.index = nTriangle;
				intersection.t = t;
				intersection.position = intersectPosition;
				intersection.normal = normal;
				intersection.material = triangles[nTriangle].material;
			}
		}
	}
	// Plane
	for (int nPlane = 0; nPlane < planes.size(); nPlane++)
	{
		v1 = planes[nPlane].vertices[1] - planes[nPlane].vertices[0];
		v2 = planes[nPlane].vertices[2] - planes[nPlane].vertices[0];
		normal = (v1 ^ v2).normalize();
		d = normal[0] * planes[nPlane].vertices[0][0] + normal[1] * planes[nPlane].vertices[0][1] + normal[2] * planes[nPlane].vertices[0][2];
		t = (d - (normal[0] * startPosition[0] + normal[1] * startPosition[1] + normal[2] * startPosition[2]))
			/ (normal[0] * ray[0] + normal[1] * ray[1] + normal[2] * ray[2]);
		intersectPosition = startPosition + ray * t;
		if (((planes[nPlane].vertices[1] - planes[nPlane].vertices[0]) ^ (intersectPosition - planes[nPlane].vertices[0])) * normal >= 0
			&& ((planes[nPlane].vertices[2] - planes[nPlane].vertices[1]) ^ (intersectPosition - planes[nPlane].vertices[1])) * normal >= 0
			&& ((planes[nPlane].vertices[3] - planes[nPlane].vertices[2]) ^ (intersectPosition - planes[nPlane].vertices[2])) * normal >= 0
			&& ((planes[nPlane].vertices[0] - planes[nPlane].vertices[3]) ^ (intersectPosition - planes[nPlane].vertices[3])) * normal >= 0)
		{
			if (t < intersection.t && t > 0)
			{
				intersection.type = 'p';
				intersection.index = nPlane;
				intersection.t = t;
				intersection.position = intersectPosition;
				intersection.normal = normal;
				intersection.material = planes[nPlane].material;
			}
		}
	}
	return intersection;
}

/*Intersection shadow(Intersection point, Light light, vector<Sphere> &spheres, vector<Triangle> &triangles, vector<Plane> &planes, CheckerBoard &checkerboard)
{
	vec3 ray = (point.position - light.position).normalize();
	Intersection intersection;
	intersection = rayTracer(light.position, ray, spheres, triangles, planes);
	if ((intersection.position - light.position).length() < (point.position - light.position).length() && !(intersection.type == point.type && intersection.index == point.index))
		point.material.color.set(0, 0, 0);
	return point;
}*/

vec3 samplingBSDF(Camera camera, vector<Light> lights, Intersection intersection, vector<Sphere> &spheres, vector<Triangle> &triangles, vector<Plane> &planes)
{
	vec3 reflectRay, ray, normal = -intersection.normal /* Inverse the normal */, rColor;
	reflectRay = (rotation3D(normal, 180) * (camera.position - intersection.position)).normalize();
	ray = reflectRay;
	Intersection sample;
	float theta, phi, attenuation;
	for (int i = 0; i < 4; i++)
	{
		theta = rand() % intersection.material.solidAngle[0] - intersection.material.solidAngle[0] / 2;
		phi = rand() % intersection.material.solidAngle[1] - intersection.material.solidAngle[1] / 2;
		ray = rotation3D(planes[intersection.index].vertices[1] - planes[intersection.index].vertices[0], theta) * ray;
		ray = rotation3D(reflectRay, phi) * ray;
		ray.normalize();
		attenuation = cos(theta / 180 * M_PI) * cos(phi / 180 * M_PI);
		sample = rayTracer(intersection.position, ray, lights, spheres, triangles, planes);
		if (sample.type == 'l' && sample.t != numeric_limits<float>::max())
		{
			rColor += sample.material.color * attenuation;
		}
		else
		{
			rColor += intersection.material.color;
		}
	}
	return rColor / 4;
}

vec3 samplingLightSource(Camera camera, vector<Light> lights, Intersection intersection, vector<Sphere> &spheres, vector<Triangle> &triangles, vector<Plane> &planes)
{
	vec3 reflectRay, ray, normal = -intersection.normal /* Inverse the normal */, rColor = intersection.material.color, projectReflectRayYZ, projectRayYZ, projectReflectRayXZ, projectRayXZ;
	Intersection lightSource;
	float theta, phi, attenuation;
	for (int i = 0; i < lights.size(); i++)
	{
		for (int j = 0; j < 4; j++)
		{
			theta = rand() % 180;
			theta = theta / 180 * M_PI;
			phi = rand() % 360;
			phi = phi / 180 * M_PI;
			ray = vec3(lights[i].radius * sin(theta) * cos(phi), lights[i].radius * sin(theta) * sin(phi), lights[i].radius * cos(theta));
			lightSource.position = lights[i].center + ray;
			lightSource.type = 'l';
			lightSource.index = i;
			lightSource.normal = ray.normalize();
			lightSource.material = lights[i].material;
			reflectRay = rotation3D(normal, 180) * (lightSource.position - intersection.position).normalize();
			ray = (camera.position - intersection.position).normalize();
			projectReflectRayYZ = reflectRay; // Project on yz-plane
			projectReflectRayYZ[0] = 0;
			projectReflectRayYZ.normalize();
			projectRayYZ = ray; // Project on yz-plane
			projectRayYZ[0] = 0;
			projectRayYZ.normalize();
			projectReflectRayXZ = reflectRay; // Project on xz-plane
			projectReflectRayXZ[1] = 0;
			projectReflectRayXZ.normalize();
			projectRayXZ = ray; // Project on xz-plane
			projectRayXZ[1] = 0;
			projectRayXZ.normalize();
			/*cout << acos(projectReflectRayYZ * projectRayYZ) * 180 / M_PI << " " << acos(projectReflectRayXZ * projectRayXZ) * 180 / M_PI << endl;
			cout << intersection.material.solidAngle[1] / 15;
			system("pause");*/
			if ((acos(projectReflectRayYZ * projectRayYZ) * 180 / M_PI < intersection.material.solidAngle[0] / 1.5)
				&& acos(projectReflectRayXZ * projectRayXZ) * 180 / M_PI < intersection.material.solidAngle[0] / 1.5)
			{
				attenuation = pow((projectReflectRayYZ * projectRayYZ) * (projectReflectRayXZ * projectRayXZ), 250);
				//if (attenuation < 0.6) system("pause");
				//cout << projectReflectRayYZ * projectRayYZ << " " << projectReflectRayXZ * projectRayXZ << " " << attenuation << endl;
				if (((rColor + lightSource.material.color * attenuation) / 2).length() > intersection.material.color.length())
				{
					rColor = (rColor + lightSource.material.color * attenuation) / 2;
				}
			}
			else
			{
				continue;//rColor += intersection.material.color;
			}
		}
	}
	return rColor;// / (4 * lights.size());
}

vec3 draw(int sampling, Camera &camera, vec3 ray, vector<Light> lights, vector<Sphere> &spheres, vector<Triangle> &triangles, vector<Plane> &planes)
{
	Intersection intersection;
	vec3 rColor = vec3(0, 0, 0);
	intersection = rayTracer(camera.position, ray, lights, spheres, triangles, planes);
	if (intersection.t != numeric_limits<float>::max())
	{
		//if (shadow(intersection[i], light, spheres, triangles, planes).material.color == vec3(0.0, 0.0, 0.0));
		rColor = intersection.material.color;
		if (intersection.type == 'p')
		{
			if (sampling == 0)
			{
				rColor = samplingBSDF(camera, lights, intersection, spheres, triangles, planes);
			}
			else
			{
				rColor = samplingLightSource(camera, lights, intersection, spheres, triangles, planes);
			}
		}
	}
	return rColor;
}

void rayTracing(int sampling, Camera &camera, Viewport &viewport, vector<Light> lights, vector<Sphere> &spheres, vector<Triangle> &triangles, vector<Plane> &planes)
{
	//calculate the center position of the viewport
	float t;
	t = sqrt(pow(viewport.distance, 2) / (pow(camera.direction[0], 2) + pow(camera.direction[1], 2) + pow(camera.direction[2], 2)));
	vec3 planeCenter = camera.position + camera.direction * t;
	//viewport plane nx * x + ny * y + nz * z = d
	float d = camera.direction[0] * planeCenter[0] + camera.direction[1] * planeCenter[1] + camera.direction[2] * planeCenter[2];
	vec3 vDownCenter = camera.direction, vLeftCenter = camera.direction;
	//find the vectors of camera to down center edge and left center edge
	vDownCenter = rotation3D(vec3(0.0, 1.0, 0.0) ^ camera.direction, camera.FOV / 2) * vDownCenter;
	vLeftCenter = rotation3D(camera.direction ^ vec3(0.0, 1.0, 0.0) ^ camera.direction, camera.FOV / 2) * vLeftCenter;
	vec3 downCenter;
	t = (d - (camera.direction[0] * camera.position[0] + camera.direction[1] * camera.position[1] + camera.direction[2] * camera.position[2]))
		/ (camera.direction[0] * vDownCenter[0] + camera.direction[1] * vDownCenter[1] + camera.direction[2] * vDownCenter[2]);
	downCenter = camera.position + vDownCenter * t;
	vec3 leftCenter;
	t = (d - (camera.direction[0] * camera.position[0] + camera.direction[1] * camera.position[1] + camera.direction[2] * camera.position[2]))
		/ (camera.direction[0] * vLeftCenter[0] + camera.direction[1] * vLeftCenter[1] + camera.direction[2] * vLeftCenter[2]);
	leftCenter = camera.position + vLeftCenter * t;
	viewport.startPosition = planeCenter - downCenter + leftCenter;
	viewport.vectorWidth = (planeCenter - leftCenter) / (viewport.width * 0.5);
	viewport.vectorHeight = (downCenter - planeCenter) / (viewport.height * 0.5);
	vec3 ray;
	for (int i = 0; i < viewport.height; i++)
	{
		for (int j = 0; j < viewport.width; j++)
		{
			ray = (viewport.startPosition + i * viewport.vectorHeight + j * viewport.vectorWidth - camera.position).normalize();// normalize the vector
			viewport.pixel[i][j] = draw(sampling, camera, ray, lights, spheres, triangles, planes);
		}
	}
}

void output(int sampling, string outputFileName[], Viewport &viewport)
{
	ColorImage image;
	Pixel p = { 0, 0, 0 };
	image.init(viewport.width, viewport.width);
	for (int i = 0; i < viewport.height; i++)
	{
		for (int j = 0; j < viewport.width; j++)
		{
			viewport.pixel[i][j] *= 255;
			for (int k = 0; k < 3; k++)
			{
				if (viewport.pixel[i][j][k] > 255)
				{
					viewport.pixel[i][j][k] = 255;
				}
			}
			p.R = viewport.pixel[i][j][0];
			p.G = viewport.pixel[i][j][1];
			p.B = viewport.pixel[i][j][2];
			image.writePixel(j, i, p);
		}
	}
	image.outputPPM(const_cast<char*>((outputFileName[sampling] + ".ppm").c_str()));
}