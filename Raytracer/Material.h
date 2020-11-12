#pragma once

#include "Header.h"
#include "Vector3.h"
#include "Ray.h"
#include "rgb.h"

class HitInfo;

class Material {
public:
	virtual bool scatter(const Ray& r_in, const HitInfo& hit, rgb& attenuation, Ray& scatterd) const = 0;
};

class Lambertian : public Material {
public:
	rgb albedo;

	Lambertian(const rgb& a) : albedo(a) {}

	virtual bool scatter(const Ray& r_in, const HitInfo& hit, rgb& attenuation, Ray& scattered) const override {
		auto scatter_direction = hit.normal + Vector3::randomUnitVecHemisphere(hit.normal);
		if (scatter_direction.near_zero())
			scatter_direction = hit.normal;
		scattered = Ray(hit.point, scatter_direction);
		attenuation = albedo;
		return true;
	}
};

class Metal : public Material {
public:
	rgb albedo;
	double fuzz;

	Metal(const rgb& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

	virtual bool scatter(const Ray& r_in, const HitInfo& hit, rgb& attenuation, Ray& scattered) const override {
		Vector3 reflected = r_in.direction.reflect(hit.normal);
		scattered = Ray(hit.point, reflected + Vector3::randomUnitVec() * fuzz);
		attenuation = albedo;
		return (scattered.direction.dot(hit.normal) > 0);
	}
};

class Dielectric : public Material {
private:
	static double reflectance(double cosine, double ref_idx) {
		auto r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
public:
	double ir;

	Dielectric(double indexOfRefract) : ir(indexOfRefract) {}

	virtual bool scatter(const Ray& r_in, const HitInfo& hit, rgb& attenuation, Ray& scattered) const override {
		attenuation = rgb(1.0, 1.0, 1.0);
		double refraction_ratio = hit.facingFront ? (1.0 / ir) : ir;

		Vector3 directionNormalized = r_in.direction.normalized();

		double cos_theta = fmin((-directionNormalized).dot(hit.normal), 1.0);
		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

		bool cannot_refract = refraction_ratio * sin_theta > 1.0;
		Vector3 direction;

		if (cannot_refract || reflectance(cos_theta, refraction_ratio) > randomDouble())
			direction = directionNormalized.reflect(hit.normal);
		else
			direction = Vector3::refract(directionNormalized, hit.normal, refraction_ratio);

		scattered = Ray(hit.point, direction);
		return true;
	}
};