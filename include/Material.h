#include <glm.hpp>

#ifndef HW3_MATERIAL
#define HW3_MATERIAL

class Material {
public:
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 emission;
	float shininess;

	Material() : diffuse(0.0), specular(0.0), emission(0.0), shininess(0.0) {}
	Material(const Material& rhs) : diffuse(rhs.diffuse), specular(rhs.specular), emission(rhs.emission), shininess(rhs.shininess) {}
	Material& operator=(const Material& rhs);
};

#endif // !HW3_MATERIAL

