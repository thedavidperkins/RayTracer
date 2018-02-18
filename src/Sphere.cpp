#include "Shape.h"
#include <iostream>

Sphere::Sphere(glm::vec3& c, float r) : 
	Shape(),
	center(c),
	radius(r)
{
	extent.xmin = c.x - r;
	extent.xmax = c.x + r;
	extent.ymin = c.y - r;
	extent.ymax = c.y + r;
	extent.zmin = c.z - r;
	extent.zmax = c.z + r;
}

float Sphere::getIntersection(Ray& ray) {
	tested = true;
	glm::vec3 p0 = ray.center;
	glm::vec3 p1 = ray.dir;
	
	// quadratic formula coefficients
	float a = glm::dot(p1, p1);
	float b = 2 * glm::dot(p1, p0 - center);
	float c = glm::dot(p0 - center, p0 - center) - radius * radius;

	float discriminant = b * b - 4 * a * c;
	float t = -1.f;
	if (discriminant < 0.f) {
		return -1.f;
	}
	else if (discriminant == 0.f) {
		t = -b / 2 / a;
	}
	else {
		t = (-sqrt(discriminant) - b) / 2 / a;
		if (t < 0.f) {
			t = (sqrt(discriminant) - b) / 2 / a;
		}
	}

	if (t > 0.f) {
		hitPoint = ray.center + t * ray.dir;
		hitNorm = glm::normalize(hitPoint - center);
	}

	return t;
}

void Sphere::dump() {
	std::cout << "dump not yet implemented for Sphere" << std::endl;
}

glm::vec3 Sphere::getPointAtHit() const {
	return hitPoint;
}

glm::vec3 Sphere::getNormalAtHit() const {
	return hitNorm;
}