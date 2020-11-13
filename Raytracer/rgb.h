#pragma once
#include <iostream>
class rgb {
public:
	float r, g, b;

	rgb() {}
	rgb(float r_, float g_, float b_) : r(r_), g(g_), b(b_) {}

	void zero() { r = 0; g = 0; b = 0; }

	rgb operator-() { return rgb(-r, -g, -b); }

	inline rgb operator+(rgb other) { return rgb(r + other.r, g + other.g, b + other.b); }
	rgb operator-(rgb other) { return rgb(r - other.r, g - other.g, b - other.b); }

	rgb operator*(rgb other) { return rgb(r * other.r, g * other.g, b * other.b); }
	rgb operator/(rgb other) { return rgb(r / other.r, g / other.g, b / other.b); }

	rgb operator*(double n) { return rgb(r * n, g * n, b * n); }
	rgb operator/(double n) { return rgb(r / n, g / n, b / n); }

	friend std::ostream& operator<<(std::ostream outstream, rgb other) {
		outstream << "(" << other.r << ", " << other.g << ", " << other.b << ")" << std::endl;
		return outstream;
	}
	
	// Zero r g b values
};