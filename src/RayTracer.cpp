#include "RayTracer.h"
#include <iostream>
#include <sstream>

const float EPS = 1e-3F;

static glm::vec3 shaveW(glm::vec4 vec) {
	return glm::vec3(vec);
}

static glm::vec4 getVecW(glm::vec3 pt) {
	return glm::vec4(pt, 0.0);
}

static glm::vec4 getPointW(glm::vec3 pt) {
	return glm::vec4(pt, 1.0);
}

static glm::vec3 applyTransfToPoint(const glm::mat4& tfm, glm::vec3 pt) {
	return shaveW(tfm * getPointW(pt));
}

static glm::vec3 applyTransfToNormal(const glm::mat4& tfm, glm::vec3 norm) {
	glm::mat3 tr3 = glm::mat3(tfm);
	return glm::normalize(glm::transpose(glm::inverse(tr3)) * norm);
}

bool RayTracer::getNextBox(glm::i16vec3& index, Ray& r) {
	EXTENT curBox = getGridBox(index);
	float xtar = r.dir.x < 0 ? curBox.xmin : curBox.xmax;
	float ytar = r.dir.y < 0 ? curBox.ymin : curBox.ymax;
	float ztar = r.dir.z < 0 ? curBox.zmin : curBox.zmax;

	float xtime = (xtar - r.center.x) / r.dir.x;
	float ytime = (ytar - r.center.y) / r.dir.y;
	float ztime = (ztar - r.center.z) / r.dir.z;
	float time;

	if (xtime < ytime) {
		if (xtime < ztime) {
			index.x += r.dir.x < 0 ? -1 : 1;
			time = xtime;
		}
		else {
			index.z += r.dir.z < 0 ? -1 : 1;
			time = ztime;
		}
	}
	else {
		if (ytime < ztime) {
			index.y += r.dir.y < 0 ? -1 : 1;
			time = ytime;
		}
		else {
			index.z += r.dir.z < 0 ? -1 : 1;
			time = ztime;
		}
	}

	r.center = r.center + r.dir * time;

	return !(index.x >= gridSize || index.y >= gridSize || index.z >= gridSize || index.x < 0 || index.y < 0 || index.z < 0);
}

EXTENT RayTracer::getGridBox(glm::i16vec3 index) {
	float xdiv = (masterExtent.xmax - masterExtent.xmin) / (1.f * gridSize);
	float ydiv = (masterExtent.ymax - masterExtent.ymin) / (1.f * gridSize);
	float zdiv = (masterExtent.zmax - masterExtent.zmin) / (1.f * gridSize);

	EXTENT ret;
	ret.xmin = masterExtent.xmin + xdiv * index.x;
	ret.xmax = ret.xmin + xdiv;
	ret.ymin = masterExtent.ymin + ydiv * index.y;
	ret.ymax = ret.ymin + ydiv;
	ret.zmin = masterExtent.zmin + zdiv * index.z;
	ret.zmax = ret.zmin + zdiv;

	return ret;
}

glm::i16vec3 RayTracer::getCoordIndex(const glm::vec3& coord) {
	int ix = (int)floorf(((coord.x - masterExtent.xmin) / (masterExtent.xmax - masterExtent.xmin)) * gridSize);
	int iy = (int)floorf(((coord.y - masterExtent.ymin) / (masterExtent.ymax - masterExtent.ymin)) * gridSize);
	int iz = (int)floorf(((coord.z - masterExtent.zmin) / (masterExtent.zmax - masterExtent.zmin)) * gridSize);

	if (ix == gridSize) ix--;
	if (iy == gridSize) iy--;
	if (iz == gridSize) iz--;

	if (ix < 0) ix = 0;
	if (iy < 0) iy = 0;
	if (iz < 0) iz = 0;

	return glm::i16vec3(ix, iy, iz);
}

void RayTracer::setUpGrid() {
	for (Shape* s : shapes) {
		glm::i16vec3 minInds = getCoordIndex(glm::vec3(s->getExtent().xmin, s->getExtent().ymin, s->getExtent().zmin));
		glm::i16vec3 maxInds = getCoordIndex(glm::vec3(s->getExtent().xmax, s->getExtent().ymax, s->getExtent().zmax));
		for (int i = minInds.x; i <= maxInds.x; ++i) {
			for (int j = minInds.y; j <= maxInds.y; ++j) {
				for (int k = minInds.z; k <= maxInds.z; ++k) {
					gridShapes[i][j][k].push_back(s);
				}
			}
		}
	}
}

RayTracer::RayTracer(int n) :
	width(0),
	height(0),
	aspect(0.0),
	gridShapes(nullptr),
	gridSize(n),
	maxDepth(5)
{
	gridShapes = new std::vector<Shape*>**[n];
	for (int i = 0; i < gridSize; ++i) {
		gridShapes[i] = new std::vector<Shape*>*[n];
		for (int j = 0; j < gridSize; ++j) {
			gridShapes[i][j] = new std::vector<Shape*>[n];
		}
	}
}

glm::vec3 RayTracer::getEyePos() {
	return eye;
}

Shape* RayTracer::findIntersection(const Ray& r, float& dist) {
	float minIntersection = INFINITY;

	Shape* winner = nullptr;

	glm::i16vec3 ind = getCoordIndex(r.center);
	Ray rCopy(r);

	std::vector<Shape*> seen;

	do {
		for (Shape* s : gridShapes[ind.x][ind.y][ind.z]) {
			if (!s->isTested()) {
				seen.push_back(s);
				glm::mat4 inverse = glm::inverse(s->getTransform());

				Ray trnRay = r.transform(inverse);

				float intersection = s->getIntersection(trnRay);
				if (intersection > 0 && intersection < minIntersection) {
					minIntersection = intersection;
					winner = s;
				}
			}
		}
	} while (getNextBox(ind, rCopy));

	for (Shape* s : seen) {
		s->reset();
	}

	dist = minIntersection;

	return winner;
}

glm::vec3 RayTracer::illuminate(const Shape* s, glm::vec3& eyeDir) {
	glm::vec3 center = s->getPointAtHit() + EPS * s->getNormalAtHit();

	center = applyTransfToPoint(s->getTransform(), center);
	glm::vec3 normal = applyTransfToNormal(s->getTransform(), s->getNormalAtHit());
	Material mat = s->getMaterial();
	glm::vec3 dir, col(0.f);

	for (Light* l : lights) {
		float dist = INFINITY;
		if (l->isDirectional()) {
			dir = l->src;
		}
		else {
			dir = l->src - center;
			dist = sqrt(glm::dot(dir, dir));
		}
		dir = glm::normalize(dir);
		
		float nearest = INFINITY;
		Ray r(dir, center);

		Shape* blocker = findIntersection(r, nearest);
		if (blocker == nullptr || nearest > dist) {
			col += l->getLightAtPoint(center, normal, dir, eyeDir, mat);
		}
	}

	return col;
}

glm::vec3 RayTracer::trace(const Ray& r, int& depth) {
	float time = 0.f;
	Shape* hit = findIntersection(r, time);

	if (hit != nullptr) {
		// Base color of surface
		glm::vec3 finColor = hit->getAmbient() + hit->getMaterial().emission;

		// Lighting
		finColor += illuminate(hit, -r.dir);

		glm::vec3 center = hit->getPointAtHit() + EPS * hit->getNormalAtHit();

		glm::vec3 normal = applyTransfToNormal(hit->getTransform(), hit->getNormalAtHit());
		center = applyTransfToPoint(hit->getTransform(), center);

		glm::vec3 refDir = r.dir - 2 * glm::dot(r.dir, normal) * normal;
		Ray refRay(refDir, center);

		// Reflections
		if (depth++ < maxDepth) {
			finColor += hit->getMaterial().specular * trace(refRay, depth);
		}

		return finColor;
	}

	return glm::vec3(0.f);
}



glm::vec3 RayTracer::getPixelRGB(int x, int y) {
	float xPos = aspect * tan(fovy / 2) * ((1.f * x - float(width) / 2.f) + 0.5f) / (float(width) / 2.0f);
	float yPos = tan(fovy / 2) * ((float(height) / 2.f - 1.f * y) - 0.5f) / (float(height) / 2.0f);

	glm::vec3 rayDir = glm::normalize(eyeDir + yPos * up + xPos * right);

	Ray ray(rayDir, eye);

	int recDepth = 0;
	return trace(ray, recDepth);
}

void RayTracer::setSize(int w, int h) {
	if (w <= 0 || h <= 0) {
		throw std::runtime_error("Error: invalid width or height");
	}
	if (width != 0) {
		delete pixels;
	}
	width = w;
	height = h;
	aspect = float(w) / float(h);
	pixels = new unsigned char[width * height * 3];
}

void RayTracer::setCamera(glm::vec3 e, glm::vec3 c, glm::vec3 u, float fvy) {
	if (aspect == 0.0) {
		throw std::runtime_error("Error: attempted to set up camera before determining screen size");
	}
	eye = e;
	center = c;
	eyeDir = glm::normalize(center - eye);
	right = glm::normalize(glm::cross(eyeDir, u));
	up = glm::normalize(glm::cross(right, eyeDir));
	fovy = glm::radians(fvy);
}

void RayTracer::addShape(Shape* s) {
	shapes.push_back(s);
}

void RayTracer::addLight(Light* l) {
	lights.push_back(l);
}

void RayTracer::computePixels() {
	setUpGrid();

	for (int i = 0; i < height; ++i) {
		std::stringstream strm;
		strm << "On row " << i << " of " << height;
		std::string progress = strm.str();
		std::cout << progress;
		for(int j = 0; j < width; ++j) {
			glm::vec3 pix = getPixelRGB(j, i);
			int index = 3 * (j + width * i);
			pixels[index] = (unsigned char) (pix.b * 255);
			pixels[index + 1] = (unsigned char) (pix.g * 255);
			pixels[index + 2] = (unsigned char) (pix.r * 255);
		}

		for (size_t iter = 0; iter < progress.size(); ++iter) {
			std::cout << (char)8;
		}
	}
}

void RayTracer::dumpCam() {
	std::cout << "Camera:" << std::endl;
	std::cout << "\tEye:" << std::endl;
	std::cout << "\t\t" << eye.x << ", " << eye.y << ", " << eye.z << std::endl;
	std::cout << "\tCenter:" << std::endl;
	std::cout << "\t\t" << center.x << ", " << center.y << ", " << center.z << std::endl;
	std::cout << "\tUp:" << std::endl;
	std::cout << "\t\t" << up.x << ", " << up.y << ", " << up.z << std::endl;
	std::cout << "\tRight:" << std::endl;
	std::cout << "\t\t" << right.x << ", " << right.y << ", " << right.z << std::endl;
	std::cout << "\tEyeDir:" << std::endl;
	std::cout << "\t\t" << eyeDir.x << ", " << eyeDir.y << ", " << eyeDir.z << std::endl;
	std::cout << std::endl;
}

void RayTracer::dumpShapes() {
	for (Shape* s : shapes) {
		s->dump();
	}
}

RayTracer::~RayTracer() {
	if (gridShapes != nullptr) {
		for (int i = 0; i < gridSize; ++i) {
			for (int j = 0; j < gridSize; ++j) {
				delete[] gridShapes[i][j];
			}
			delete[] gridShapes[i];
		}
		delete[] gridShapes;
	}
	for (Shape* s : shapes) {
		delete s;
	}
	for (Light* l : lights) {
		delete l;
	}
	if (width != 0) {
		delete pixels;
	}
}