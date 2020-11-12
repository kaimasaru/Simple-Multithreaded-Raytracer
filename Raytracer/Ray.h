#pragma once
#include "Vector3.h"
class Ray {
public:
	Vector3 origin, direction;
	Ray() {}
	Ray(Vector3& s, Vector3& e) : origin(s), direction(e) {}
	Ray(Vector3 s, Vector3 e) : origin(s), direction(e) {}
	Vector3 at(float t) const { return origin + direction * t; }
};