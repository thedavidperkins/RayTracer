#include <string>
#include <vector>
#include <glm.hpp>
#include "Shape.h"
#include "Light.h"

#ifndef HW3_RAYTRACER
#define HW3_RAYTRACER

class RayTracer {
private:
	int gridSize;
	std::vector<Shape*>*** gridShapes;
	EXTENT masterExtent;

	ShapeList shapes;
	LightList lights;
	unsigned char* pixels;
	int width, height;
	float aspect;
	glm::vec3 eye, center, up, eyeDir, right;
	float fovy;
	
	glm::vec3 getPixelRGB(int x, int y);
	Shape* findIntersection(Ray& r, float& dist);
	glm::vec3 illuminate(const Shape* s, glm::vec3& eyeDir);

	glm::vec3 reflect(const Ray& r, Shape* s, int& depth);
	bool getNextBox(glm::i16vec3& index, Ray& r);
	EXTENT getGridBox(glm::i16vec3 index);
	glm::i16vec3 getCoordIndex(const glm::vec3& coord);
	void setUpGrid();
public:
	int maxDepth;
	std::string outFileName;
	EXTENT& getExtent() { return masterExtent; }

	RayTracer(int n);

	void setSize(int width, int height);
	void setCamera(glm::vec3 e, glm::vec3 c, glm::vec3 u, float fvy);
	glm::vec3 getEyePos();

	void addShape(Shape* s);
	void addLight(Light* l);

	void computePixels();
	unsigned char* getPixels() { return pixels; }
	int getWidth() { return width; }
	int getHeight() { return height; }

	void dumpCam();
	void dumpShapes();

	~RayTracer();
};

#endif
