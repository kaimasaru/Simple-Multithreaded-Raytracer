#pragma once
#include <iostream>
class rgb {
public:
	float r, g, b;

	rgb() {}
	rgb(float r_, float g_, float b_) : r(r_), g(g_), b(b_) {}

	inline rgb operator-() { return rgb(-r, -g, -b); }

	inline rgb operator+(rgb other) { return rgb(r + other.r, g + other.g, b + other.b); }
	inline rgb operator-(rgb other) { return rgb(r - other.r, g - other.g, b - other.b); }

	inline rgb operator*(rgb other) { return rgb(r * other.r, g * other.g, b * other.b); }
	inline rgb operator/(rgb other) { return rgb(r / other.r, g / other.g, b / other.b); }

	inline rgb operator*(double n) { return rgb(r * n, g * n, b * n); }
	inline rgb operator/(double n) { return rgb(r / n, g / n, b / n); }

	friend std::ostream& operator<<(std::ostream outstream, rgb other) {
		outstream << "(" << other.r << ", " << other.g << ", " << other.b << ")" << std::endl;
		return outstream;
	}
};