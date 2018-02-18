#include <glm.hpp>

namespace Transform {
	glm::mat4 translate(float x, float y, float z) {
		glm::mat4 ret = { { 1, 0, 0, x },
		                  { 0, 1, 0, y },
						  { 0, 0, 1, z },
						  { 0, 0, 0, 1 } };

		return glm::transpose(ret);
	}

	glm::mat4 rotate(glm::vec3& axis, float angle) {
		glm::vec3 normAxis = glm::normalize(axis);

		glm::mat3 id(1.0);

		glm::mat3 aTa = { { normAxis.x * normAxis.x,	normAxis.x * normAxis.y,	normAxis.x * normAxis.z },
						  { normAxis.y * normAxis.x,	normAxis.y * normAxis.y,	normAxis.y * normAxis.z },
						  { normAxis.z * normAxis.x,	normAxis.z * normAxis.y,	normAxis.z * normAxis.z } };

		glm::mat3 aDual = { {           0, -normAxis.z,  normAxis.y },
							{  normAxis.z,           0, -normAxis.x },
							{ -normAxis.y,  normAxis.x,           0 } };

		float radAngle = glm::radians(angle);

		glm::mat3 rot = id * cos(radAngle) + aTa * (1 - cos(radAngle)) + aDual * sin(radAngle);
		glm::mat4 ret(rot);
		return glm::transpose(ret);
	}

	glm::mat4 scale(float x, float y, float z) {
		glm::mat4 ret = { { x, 0, 0, 0 },
		                  { 0, y, 0, 0 },
						  { 0, 0, z, 0 },
						  { 0, 0, 0, 1 } };

		return ret;
	}
}