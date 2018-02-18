#include <glm.hpp>
#include <list>
#include "Ray.h"
#include "Material.h"

#ifndef HW3_SHAPES
#define HW3_SHAPES

struct EXTENT {
	float xmin;
	float xmax;
	float ymin;
	float ymax;
	float zmin;
	float zmax;

	EXTENT() : xmin(0.f), xmax(0.f), ymin(0.f), ymax(0.f), zmin(0.f), zmax(0.f) {}
};

class Shape {
protected:
	glm::mat4 transform;
	glm::vec3 ambient;
	Material mat;
	EXTENT extent;
	bool tested;

	void transformExtent();

public:
	Shape() : ambient(0.2, 0.2, 0.2), transform(1.0), tested(false) {}

	virtual float getIntersection(Ray& r) = 0;
	virtual void dump() = 0;
	void setTransform(glm::mat4& t);
	glm::mat4 getTransform() const;
	void setAmbient(float R, float G, float B);
	void setMaterial(const Material& m);
	Material getMaterial() const;
	void reset() { tested = false; }
	bool isTested() { return tested; }
	EXTENT getExtent() { return extent; }

	glm::vec3 getAmbient();

	virtual glm::vec3 getNormalAtHit() const = 0;
	virtual glm::vec3 getPointAtHit() const = 0;
};

class Sphere : public Shape {
private:
	float radius;
	glm::vec3 center, hitPoint, hitNorm;
public:
	Sphere(glm::vec3& c, float r);
	virtual float getIntersection(Ray& r);
	virtual glm::vec3 getPointAtHit() const;
	virtual glm::vec3 getNormalAtHit() const;
	virtual void dump();
};

class Triangle : public Shape {
private:
	glm::vec3 normal;
protected:
	glm::vec3 a, b, c;
	float hitAlpha, hitBeta, hitGamma;

public:
	Triangle(glm::vec3& A, glm::vec3& B, glm::vec3& C);
	virtual float getIntersection(Ray& r);
	virtual glm::vec3 getPointAtHit() const;
	virtual glm::vec3 getNormalAtHit() const;
	virtual void dump();
};

class TriNormal : public Triangle {
private:
	glm::vec3 na, nb, nc;
public:
	TriNormal(glm::vec3& A, glm::vec3& B, glm::vec3& C, glm::vec3& nA, glm::vec3& nB, glm::vec3& nC);
	virtual glm::vec3 getNormalAtHit() const;
	virtual void dump();
};

class ShapeList : public std::list<Shape*> {};

#endif
