#include "Ray.h"

Ray Ray::transform(const glm::mat4& tfm) const {
	glm::vec4 c4 = glm::vec4(center.x, center.y, center.z, 1.f);
	glm::vec4 d4 = glm::vec4(dir.x, dir.y, dir.z, 0.f);
	c4 = tfm * c4;
	d4 = tfm * d4;
	return Ray(glm::vec3(d4.x, d4.y, d4.z), glm::vec3(c4.x, c4.y, c4.z));
}