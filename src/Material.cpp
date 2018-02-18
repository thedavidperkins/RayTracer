#include "Material.h"

Material& Material::operator=(const Material& rhs) {
	this->diffuse = rhs.diffuse;
	this->emission = rhs.emission;
	this->shininess = rhs.shininess;
	this->specular = rhs.specular;
	return *this;
}