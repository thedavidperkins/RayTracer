#include "Shape.h"
#include <iostream>

Triangle::Triangle(glm::vec3& A, glm::vec3& B, glm::vec3& C) :
	Shape(),
	a(A),
	b(B),
	c(C),
	hitAlpha(0.f),
	hitBeta(0.f),
	hitGamma(0.f)
{
	glm::vec3 side0 = a - b;
	glm::vec3 side1 = a - c;
	normal = glm::normalize(glm::cross(side0, side1));

	float xMax = a.x > b.x ? a.x : b.x;
	extent.xmax = xMax > c.x ? xMax : c.x;
	float xMin = a.x < b.x ? a.x : b.x;
	extent.xmin = xMin < c.x ? xMin : c.x;

	float yMax = a.y > b.y ? a.y : b.y;
	extent.ymax = yMax > c.y ? yMax : c.y;
	float yMin = a.y < b.y ? a.y : b.y;
	extent.ymin = yMin < c.y ? yMin : c.y;

	float zMax = a.z > b.z ? a.z : b.z;
	extent.zmax = zMax > c.z ? zMax : c.z;
	float zMin = a.z < b.z ? a.z : b.z;
	extent.zmin = zMin < c.z ? zMin : c.z;
}

float Triangle::getIntersection(Ray& ray) {
	tested = true;
	float denom = glm::dot(ray.dir, normal);
	if (denom == 0.f) {
		return -1.f;
	}
	float t = (glm::dot(a, normal) - glm::dot(ray.center, normal)) / denom;

	glm::vec3 p = ray.center + t * ray.dir;

	// Determine whether p is in triangle -- barycentric parametrization
	float beta, gamma;
	glm::vec3 betaVec = b - a;
	glm::vec3 gammaVec = c - a;
	glm::vec3 pVec = p - a;
	int i;
	for (i = 0; i < 3; ++i) {
		if (betaVec[i] == 0.f && gammaVec[i] != 0.f) {
			gamma = pVec[i] / gammaVec[i];
			for (int j = 0; j < 3; ++j) {
				if (j == i) continue;
				if (betaVec[j] != 0.f) {
					beta = (pVec[j] - gamma * gammaVec[j]) / betaVec[j];
					break;
				}
			}
			break;
		}
		if (betaVec[i] != 0.f && gammaVec[i] == 0.f) {
			beta = pVec[i] / betaVec[i];
			for (int j = 0; j < 3; ++j) {
				if (j == i) continue;
				if (gammaVec[j] != 0.f) {
					gamma = (pVec[j] - beta * betaVec[j]) / gammaVec[j];
					break;
				}
			}
			break;
		}
	}
	if (i == 3) {
		for (i = 0; i < 3; ++i) {
			if (betaVec[i] != 0.f) {
				for (int j = 0; j < 3; ++j) {
					if (j == i) continue;
					if (betaVec[j] != 0.f) {
						gamma = (betaVec[i] * pVec[j] - betaVec[j] * pVec[i]) / (betaVec[i] * gammaVec[j] - betaVec[j] * gammaVec[i]);
						beta = (pVec[i] - gamma * gammaVec[i]) / betaVec[i];
						break;
					}
				}
				break;
			}
		}
	}
	if (i == 3) {
		return -1.f;
	}

	if (beta >= 0.f && beta <= 1.f && gamma >= 0.f && gamma <= 1.f && beta + gamma <= 1.f) {
		hitBeta = beta;
		hitGamma = gamma;
		hitAlpha = 1.f - beta - gamma;
		return t;
	}

	return -1.f;
}

glm::vec3 Triangle::getPointAtHit() const {
	return hitAlpha * a + hitBeta * b + hitGamma * c;
}

glm::vec3 Triangle::getNormalAtHit() const {
	return normal;
}

void Triangle::dump() {
	std::cout << "Triangle: " << std::endl;
	std::cout << "\tVertices: " << std::endl;
	std::cout << "\t\t" << a.x << ", " << a.y << ", " << a.z << std::endl;
	std::cout << "\t\t" << b.x << ", " << b.y << ", " << b.z << std::endl;
	std::cout << "\t\t" << c.x << ", " << c.y << ", " << c.z << std::endl;
	std::cout << "\tAmbient color: " << std::endl;
	std::cout << "\t\t" << ambient.r << ", " << ambient.g << ", " << ambient.b << std::endl;
	std::cout << "\tNormal: " << std::endl;
	std::cout << "\t\t" << normal.r << ", " << normal.g << ", " << normal.b << std::endl;
	std::cout << std::endl;
}

TriNormal::TriNormal(glm::vec3& A, glm::vec3& B, glm::vec3& C, glm::vec3& nA, glm::vec3& nB, glm::vec3& nC) :
	Triangle(A, B, C),
	na(nA),
	nb(nB),
	nc(nC) {}

glm::vec3 TriNormal::getNormalAtHit() const {
	return hitAlpha * na + hitBeta * nb + hitGamma * nc;
}

void TriNormal::dump() {
	Triangle::dump();
}