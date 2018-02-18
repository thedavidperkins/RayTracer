#include <glm.hpp>

#ifndef HW3_RAY
#define HW3_RAY

class Ray {
public:
	glm::vec3 dir, center;

	Ray(glm::vec3 d, glm::vec3 c) : dir(d), center(c) {}
	Ray(const Ray& rhs) : dir(rhs.dir), center(rhs.center) {}

	Ray transform(const glm::mat4&);
};

#endif
