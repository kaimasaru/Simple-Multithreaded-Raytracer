#pragma once
#include "Ray.h"
#include <memory>

class Material;

class HitInfo {
public:
	Vector3 point;
	Vector3 normal;
	double t;
	bool facingFront;
	std::shared_ptr<Material> material;

	inline void setFaceNormal(const Ray& r, Vector3& outNormal) {
		facingFront = r.direction.dot(outNormal) < 0;
		normal = facingFront ? outNormal : -outNormal;
	}
};

class Object {
public:
	virtual bool intersect(const Ray&, double, double, HitInfo& hit) = 0;
};

class ObjectList : public Object{
public:

	ObjectList() {}
	ObjectList(std::shared_ptr<Object> object) { add(object); }

	void clear() { objects.clear(); }
	void add(std::shared_ptr<Object> object) { objects.push_back(object); }

	bool intersect(const Ray& r, double t_min, double t_max, HitInfo& hit) override {
		HitInfo temp;
		bool hitAnything = false;
		auto closestT = t_max;

		for (const auto& object : objects) {
			if (object->intersect(r, t_min, closestT, temp)) {
				hitAnything = true;
				closestT = temp.t;
				hit = temp;
			}
		}

		return hitAnything;
	}

	std::vector<std::shared_ptr<Object>> objects;
};