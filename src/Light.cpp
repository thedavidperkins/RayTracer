#include "Light.h"
#include "RayTracer.h"

float PointLight::getAttenuation(glm::vec3 loc) {
	glm::vec3 displacement = loc - src;
	float distance2 = glm::dot(displacement, displacement);
	float distance = sqrt(distance2);

	return 1.f / (c0 + distance * c1 + distance2 * c2);
}

glm::vec3 Light::getLightAtPoint(glm::vec3 point, glm::vec3 normal, glm::vec3 dir, glm::vec3 eyeDir, Material mat) {
	float nDotL = fmax(glm::dot(normal, dir), 0.f);
	glm::vec3 lambert = nDotL * mat.diffuse * col;
	glm::vec3 halfVec = glm::normalize(dir + eyeDir);
	float nDotH = fmax(glm::dot(normal, halfVec), 0.f);
	glm::vec3 phong = mat.specular * col * powf(nDotH, mat.shininess);
	return lambert + phong;
}