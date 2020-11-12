#pragma once
class Vector3 {
public:
	float x, y, z;
	Vector3() : x(0), y(0), z(0) {}
	Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

	inline Vector3 operator-() const { return Vector3(-x, -y, -z); }
	// Addition/Subtraction (Vector3)
	Vector3 operator-(const Vector3& other) const { return Vector3(x - other.x, y - other.y, z - other.z); }
	Vector3 operator+(const Vector3& other) const { return Vector3(x + other.x, y + other.y, z + other.z); }
	// Multiply/Divide (Vector3)
	Vector3 operator*(const Vector3& other) const { return Vector3(x * other.x, y * other.y, z * other.z); }
	Vector3 operator/(const Vector3& other) const { return Vector3(x / other.x, y / other.y, z / other.z); }
	// Multiply/Divide (float)
	Vector3 operator*(double n) const { return Vector3(x * n, y * n, z * n); }
	Vector3 operator/(double n) const { return Vector3(x / n, y / n, z / n); }

	inline float lengthSquared() const { return dot(*this); }

	// Absolute value magnitude using distance formula
	inline float magnitude() const { return fabs(sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2))); }

	// Dot product
	inline float dot(const Vector3& other) const { return (x * other.x) + (y * other.y) + (z * other.z); }

	inline Vector3 normalized() const { return magnitude() != 0 ? *this / magnitude() : *this; }

	inline void print() { std::cout << "(" << x << ", " << y << ", " << z << ")\n"; }

	// Negate
	const Vector3 operator-() { return Vector3(-x, -y, -z); }

	bool near_zero() const {
		const auto s = 1e-8;
		return (fabs(x) < s) && (fabs(y) < s) && (fabs(z) < s);
	}

	inline Vector3 reflect(const Vector3& n) const {
		return *this - (n * this->dot(n) * 2);
	}

	inline static Vector3 refract(const Vector3& uv, const Vector3& n, double etai_over_etat) {
		auto cos_theta = fmin((-uv).dot(n), 1.0);
		Vector3 r_out_perp = (uv + n * cos_theta) * etai_over_etat;
		Vector3 r_out_parallel = n * -sqrtf(fabs(1.0 - r_out_perp.lengthSquared()));
		return r_out_perp + r_out_parallel;
	}

	inline static Vector3 randomUnitVec() {
		while (true) {
			auto p = Vector3(randomDouble(), randomDouble(), randomDouble());
			if (p.lengthSquared() >= 1) continue;
			return p;
		}
	}

	inline static Vector3 randomUnitVecHemisphere(const Vector3& normal) {
		Vector3 unitHemi = randomUnitVec();
		if (unitHemi.dot(normal) > 0.0) {
			return unitHemi;
		}
		else {
			return -unitHemi;
		}
	}

	friend std::ostream& operator<<(std::ostream& outstream, Vector3 other) {
		outstream << "(" << other.x << ", " << other.y << ", " << other.z << ")";
		return outstream;
	}
};