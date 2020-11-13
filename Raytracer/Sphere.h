#pragma once
#include "Object.h"
#include "Ray.h"
#include "Vector3.h"
#include "Material.h"
class Sphere : public Object {
private:
	Vector3 center;
	double radius;
	std::shared_ptr<Material> material;

public:
	Sphere() : radius(0) {}
	Sphere(Vector3 center_, double radius_, std::shared_ptr<Material> m) : center(center_), radius(radius_), material(m) {}

	bool intersect(const Ray& r, double t_min, double t_max, HitInfo& hit) override {
		Vector3 oc = r.origin - center;
		auto a = r.direction.lengthSquared();
		auto half_b = oc.dot(r.direction);
		auto c = oc.lengthSquared() - radius * radius;
		auto discriminant = (half_b * half_b) - a * c;
	
		if (discriminant < 0) { return false; }
		auto sqrtd = sqrt(discriminant);

		auto root = (-half_b - sqrtd) / a;
		if (root < t_min || root > t_max) {
			root = (-half_b + sqrtd) / a;
			if (root < t_min || root > t_max) {
				return false;
			}
		}

		hit.t = root;
		hit.point = r.at(hit.t);
		Vector3 outNormal = (hit.point - center).normalized();
		hit.setFaceNormal(r, outNormal);
		hit.material = material;
		return true;
	}

};