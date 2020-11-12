#pragma once
#include "Vector3.h"
#include <math.h>

class Camera {
public:
	float viewX;
	float viewY;
	float aspectRatio;
	float scale;

	Vector3 position;

	Camera(float x, float y, float fov) : viewX(x), viewY(y), aspectRatio(x / y), scale(tan(((fov * 4.0 * atan(1.0)) / 360))){}

	Vector3 pixelToWorld(float a, float b) {
		auto x = (2 * (a + 0.5) / viewX - 1) * aspectRatio * scale;
		auto y = (1 - 2 * (b + 0.5) / viewY) * scale;

		return Vector3(x, y, -1);
	}
};