#pragma once
#include <math.h>
#include <random>
#include <iostream>

#define PI 3.1415926535897932385

float clamp(float min, float max, float n) {
	if (n > max) {
		return max;
	}
	else if (n < min) {
		return min;
	}
	return n;
}

float clamp(float& n, float max) {
	if (n > max) {
		return max;
	}
	else if (n < 0) {
		return 0;
	}
	return n;
}

inline int randomInt() {
	static std::uniform_int_distribution<int> distribution(-1, 1);
	static::std::mt19937 generator;
	return distribution(generator);
}

inline double randomDouble() {
	static std::uniform_real_distribution<double> distribution(-1, 1);
	static::std::mt19937 generator;
	return distribution(generator);
}

inline double randomdub() {
	static std::uniform_real_distribution<double> distribution(-0.3, 0.3);
	static::std::mt19937 generator;
	return distribution(generator);
}

inline double deg2Rad(double degrees) {
	return degrees * PI / 100.0;
}