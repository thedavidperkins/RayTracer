#include "Shape.h"
#include "Utils.h"

void Shape::setTransform(glm::mat4& t) {
	transform = t;
	transformExtent();
}

void Shape::transformExtent() {
	glm::vec4 x0y0z0 = glm::vec4(extent.xmin, extent.ymin, extent.zmin, 1.f);
	glm::vec4 x0y0z1 = glm::vec4(extent.xmin, extent.ymin, extent.zmax, 1.f);
	glm::vec4 x0y1z0 = glm::vec4(extent.xmin, extent.ymax, extent.zmin, 1.f);
	glm::vec4 x0y1z1 = glm::vec4(extent.xmin, extent.ymax, extent.zmax, 1.f);
	glm::vec4 x1y0z0 = glm::vec4(extent.xmax, extent.ymin, extent.zmin, 1.f);
	glm::vec4 x1y0z1 = glm::vec4(extent.xmax, extent.ymin, extent.zmax, 1.f);
	glm::vec4 x1y1z0 = glm::vec4(extent.xmax, extent.ymax, extent.zmin, 1.f);
	glm::vec4 x1y1z1 = glm::vec4(extent.xmax, extent.ymax, extent.zmax, 1.f);

	glm::vec4 x0y0z0t = transform * x0y0z0;
	glm::vec4 x0y0z1t = transform * x0y0z1;
	glm::vec4 x0y1z0t = transform * x0y1z0;
	glm::vec4 x0y1z1t = transform * x0y1z1;
	glm::vec4 x1y0z0t = transform * x1y0z0;
	glm::vec4 x1y0z1t = transform * x1y0z1;
	glm::vec4 x1y1z0t = transform * x1y1z0;
	glm::vec4 x1y1z1t = transform * x1y1z1;

	extent.xmin = vmin(x0y0z0t.x, x0y0z1t.x, x0y1z0t.x, x0y1z1t.x, x1y0z0t.x, x1y0z1t.x, x1y1z0t.x, x1y1z1t.x);
	extent.xmax = vmax(x0y0z0t.x, x0y0z1t.x, x0y1z0t.x, x0y1z1t.x, x1y0z0t.x, x1y0z1t.x, x1y1z0t.x, x1y1z1t.x);
	extent.ymin = vmin(x0y0z0t.y, x0y0z1t.y, x0y1z0t.y, x0y1z1t.y, x1y0z0t.y, x1y0z1t.y, x1y1z0t.y, x1y1z1t.y);
	extent.ymax = vmax(x0y0z0t.y, x0y0z1t.y, x0y1z0t.y, x0y1z1t.y, x1y0z0t.y, x1y0z1t.y, x1y1z0t.y, x1y1z1t.y);
	extent.zmin = vmin(x0y0z0t.z, x0y0z1t.z, x0y1z0t.z, x0y1z1t.z, x1y0z0t.z, x1y0z1t.z, x1y1z0t.z, x1y1z1t.z);
	extent.zmax = vmax(x0y0z0t.z, x0y0z1t.z, x0y1z0t.z, x0y1z1t.z, x1y0z0t.z, x1y0z1t.z, x1y1z0t.z, x1y1z1t.z);
}

glm::mat4 Shape::getTransform() const {
	return transform;
}

void Shape::setAmbient(float R, float G, float B) {
	ambient = glm::vec3(R, G, B);
}

void Shape::setMaterial(const Material& m) {
	mat = m;
}

Material Shape::getMaterial() const {
	return mat;
}

glm::vec3 Shape::getAmbient() {
	return ambient;
}