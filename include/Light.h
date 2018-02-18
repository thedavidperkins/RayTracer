#include <list>
#include <glm.hpp>
#include "Material.h"

#ifndef HW3_LIGHTS
#define HW3_LIGHTS

class Light {
protected:


public:
	glm::vec3 src, col;
	Light(float X, float Y, float Z, float R, float G, float B) : src(X, Y, Z), col(R, G, B) {}
	virtual float getAttenuation(glm::vec3 loc) { return 1.f; };
	virtual bool isDirectional() { return false; }
	glm::vec3 getLightAtPoint(glm::vec3 point, glm::vec3 normal, glm::vec3 dir, glm::vec3 eyeDir, Material mat);
};

class DirectionalLight : public Light {
public:
	DirectionalLight(float X, float Y, float Z, float R, float G, float B) : Light(X, Y, Z, R, G, B) {}
	virtual bool isDirectional() { return true; }
};

class PointLight : public Light {
private:
	float c0, c1, c2;
public:
	PointLight(float X, float Y, float Z, float R, float G, float B) : Light(X, Y, Z, R, G, B), c0(1.f), c1(0.f), c2(0.f) {}
	void setAttenuation(float C0, float C1, float C2) { c0 = C0; c1 = C1; c2 = C2; }
	virtual float getAttenuation(glm::vec3 loc);
};

class LightList : public std::list<Light*> {};

#endif
