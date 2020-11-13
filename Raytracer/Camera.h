#pragma once
#include "Vector3.h"
#include <math.h>

class Camera {
private:
	Vector3 origin;
	Vector3 lower_left_corner;
	Vector3 horizontal;
	Vector3 vertical;
public:
	float viewX;
	float viewY;
	float aspectRatio;
	float scale;

	Vector3 position;
	Vector3 look;
	Vector3 up;

	Camera(Vector3 position_, Vector3 look_, Vector3 up_, float x, float y, float fov) : position(position_), 
		look(look_), 
		up(up_), 
		viewX(x), 
		viewY(y), 
		aspectRatio(x / y), 
		scale(tan(((fov * 4.0 * atan(1.0)) / 360))) 
	{
		auto w = (position - look).normalized();
		auto u = (up.cross(w)).normalized();
		auto v = w.cross(u);

		origin = position;
		horizontal = u * viewX * aspectRatio * scale;
		vertical = v * viewY * scale;
		lower_left_corner = origin - horizontal / 2 - vertical / 2 - w;
	}

	Vector3 pixelToWorld(float a, float b) {
		auto x = (2 * (a + 0.5) / viewX - 1) * aspectRatio * scale;
		auto y = (1 - 2 * (b + 0.5) / viewY) * scale;

		return Vector3(x, y, -1);
	}

	Ray getRay(double s, double t) const {
		return Ray(origin, lower_left_corner + horizontal * s + vertical * t - origin);
	}
};