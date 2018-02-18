#include <glm.hpp>

#ifndef HW3_TRANSFORM
#define HW3_TRANSFORM

namespace Transform {
	glm::mat4 translate(float x, float y, float z);
	glm::mat4 rotate(glm::vec3& axis, float angle);
	glm::mat4 scale(float x, float y, float z);
}

#endif // !HW3_TRANSFORM

