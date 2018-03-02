#include "Shape.h"
#include "RayTracer.h"
#include "Transform.h"
#include <glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stack>

static bool extentNotSet = true;

static void updateExtent(EXTENT& master, EXTENT newEx) {
	if (extentNotSet) {
		master = newEx;
		extentNotSet = false;
	}

	master.xmax = master.xmax > newEx.xmax ? master.xmax : newEx.xmax;
	master.xmin = master.xmin < newEx.xmin ? master.xmin : newEx.xmin;
	master.ymax = master.ymax > newEx.ymax ? master.ymax : newEx.ymax;
	master.ymin = master.ymin < newEx.ymin ? master.ymin : newEx.ymin;
	master.zmax = master.zmax > newEx.zmax ? master.zmax : newEx.zmax;
	master.zmin = master.zmin < newEx.zmin ? master.zmin : newEx.zmin;
}

bool readFile(const std::string& fileName, RayTracer& rt) {
	std::ifstream strm(fileName);
	if (strm) {
		std::string line, word;
		std::getline(strm, line);

		int intVals[10];
		float floatVals[10];

		glm::vec3* vertices = nullptr;
		glm::vec3* vertWithNorms = nullptr;
		glm::vec3* normals = nullptr;

		int maxverts = -1, vertsAdded = 0;
		int maxvertnormals = -1, vertsNormalAdded = 0;

		std::stack<glm::mat4> transfStack;
		transfStack.push(glm::mat4(1.0));

		// Lighting properties -- attenuation coefficients and ambient colors
		float c0, c1, c2;
		float r, g, b;
		bool attSet = false;
		bool ambSet = false;

		// Material properties
		Material mater;

		// Main parsing loop
		while (strm) {
			if ((line.find_first_not_of(" \t\r\n") != std::string::npos) && (line[0] != '#')) {
				std::stringstream strStrm(line);
				strStrm >> word;
				if (word == "size") {
					strStrm >> intVals[0];
					strStrm >> intVals[1];
					rt.setSize(intVals[0], intVals[1]);
				}
				else if (word == "maxdepth") {
					strStrm >> rt.maxDepth;
				}
				else if (word == "output") {
					strStrm >> rt.outFileName;
				}
				else if (word == "camera") {
					for (int iter = 0; iter < 10; ++iter) {
						strStrm >> floatVals[iter];
					}
					rt.setCamera(glm::vec3(floatVals[0], floatVals[1], floatVals[2]), glm::vec3(floatVals[3], floatVals[4], floatVals[5]), glm::vec3(floatVals[6], floatVals[7], floatVals[8]), floatVals[9]);
				}
				else if (word == "maxverts") {
					strStrm >> maxverts;
					vertices = new glm::vec3[maxverts];
				}
				else if (word == "maxvertnorms") {
					strStrm >> maxvertnormals;
					vertWithNorms = new glm::vec3[maxvertnormals];
					normals = new glm::vec3[maxvertnormals];
				}
				else if (word == "vertex") {
					if (maxverts - vertsAdded > 0) {
						for (int iter = 0; iter < 3; ++iter) {
							strStrm >> floatVals[iter];
						}
						glm::vec3 vert(floatVals[0], floatVals[1], floatVals[2]);
						vertices[vertsAdded] = vert;
						vertsAdded++;
					}
					else {
						throw std::runtime_error("Error: attempted to add more vertices than promised");
					}
				}
				else if (word == "vertexnormal") {
					if (maxvertnormals - vertsNormalAdded > 0) {
						for (int iter = 0; iter < 6; ++iter) {
							strStrm >> floatVals[iter];
						}
						glm::vec3 vert(floatVals[0], floatVals[1], floatVals[2]);
						glm::vec3 norm(floatVals[3], floatVals[4], floatVals[5]);
						vertWithNorms[vertsNormalAdded] = vert;
						normals[vertsNormalAdded] = norm;
						vertsNormalAdded++;
					}
					else {
						throw std::runtime_error("Error: attempted to add more vertices with normals than promised");
					}
				}
				else if (word == "sphere" || word == "tri" || word == "trinormal") {
					Shape* s;
					if (word == "sphere") {
						for (int iter = 0; iter < 4; iter++) {
							strStrm >> floatVals[iter];
						}
					    glm::vec3 center(floatVals[0], floatVals[1], floatVals[2]);
						s = new Sphere(center, floatVals[3]);
					}
					else if (word == "tri") {
						for (int iter = 0; iter < 3; ++iter) {
							strStrm >> intVals[iter];
							if (intVals[iter] >= vertsAdded) {
								throw std::runtime_error("Error: attempted to add a triangle with an invalid vertex");
							}
						}
						s = new Triangle(vertices[intVals[0]], vertices[intVals[1]], vertices[intVals[2]]);
					}
					else {
						for (int iter = 0; iter < 3; ++iter) {
							strStrm >> intVals[iter];
							if (intVals[iter] >= vertsNormalAdded) {
								throw std::runtime_error("Error: attempted to add a trinormal with an invalid vertex");
							}
						}
						s = new TriNormal(vertices[intVals[0]], vertices[intVals[1]], vertices[intVals[2]], normals[intVals[0]], normals[intVals[1]], normals[intVals[2]]);
					}
					s->setTransform(transfStack.top());
					s->setMaterial(mater);
					if (ambSet) {
						s->setAmbient(r, g, b);
					}
					updateExtent(rt.getExtent(), s->getExtent());
					rt.addShape(s);
				}
				else if (word == "translate") {
					for (int iter = 0; iter < 3; iter++) {
						strStrm >> floatVals[iter];
					}
					transfStack.top() *= Transform::translate(floatVals[0], floatVals[1], floatVals[2]);
				}
				else if (word == "rotate") {
					for (int iter = 0; iter < 4; iter++) {
						strStrm >> floatVals[iter];
					}
					transfStack.top() *= Transform::rotate(glm::vec3(floatVals[0], floatVals[1], floatVals[2]), floatVals[3]);
				}
				else if (word == "scale") {
					for (int iter = 0; iter < 3; iter++) {
						strStrm >> floatVals[iter];
					}
					transfStack.top() *= Transform::scale(floatVals[0], floatVals[1], floatVals[2]);
				}
				else if (word == "pushTransform") {
					transfStack.push(transfStack.top());
				}
				else if (word == "popTransform") {
					if (transfStack.size() <= 1) {
						throw std::runtime_error("Error: attempted to pop too many transforms");
					}
					else {
						transfStack.pop();
					}
				}
				else if (word == "directional") {
					for (int iter = 0; iter < 6; ++iter) {
						strStrm >> floatVals[iter];
					}
					Light* l = new DirectionalLight(floatVals[0], floatVals[1], floatVals[2], floatVals[3], floatVals[4], floatVals[5]);
					rt.addLight(l);
				}
				else if (word == "point") {
					for (int iter = 0; iter < 6; ++iter) {
						strStrm >> floatVals[iter];
					}
					PointLight* l = new PointLight(floatVals[0], floatVals[1], floatVals[2], floatVals[3], floatVals[4], floatVals[5]);
					if (attSet) {
						l->setAttenuation(c0, c1, c2);
					}
					rt.addLight(l);
				}
				else if (word == "attenuation") {
					strStrm >> c0;
					strStrm >> c1;
					strStrm >> c2;
					attSet = true;
				}
				else if (word == "ambient") {
					strStrm >> r;
					strStrm >> g;
					strStrm >> b;
					ambSet = true;
				}
				else if (word == "diffuse") {
					for (int iter = 0; iter < 3; ++iter) {
						strStrm >> floatVals[iter];
					}
					mater.diffuse = glm::vec3(floatVals[0], floatVals[1], floatVals[2]);
				}
				else if (word == "specular") {
					for (int iter = 0; iter < 3; ++iter) {
						strStrm >> floatVals[iter];
					}
					mater.specular = glm::vec3(floatVals[0], floatVals[1], floatVals[2]);
				}
				else if (word == "shininess") {
					strStrm >> mater.shininess;
				}
				else if (word == "emission") {
					for (int iter = 0; iter < 3; ++iter) {
						strStrm >> floatVals[iter];
					}
					mater.emission = glm::vec3(floatVals[0], floatVals[1], floatVals[2]);
				}
			}
			std::getline(strm, line);
		}

		if (maxverts > 0) {
			delete vertices;
		}
		if (maxvertnormals > 0) {
			delete vertWithNorms;
			delete normals;
		}
	}
	else {
		std::cerr << "Error: file not found" << std::endl;
		return false;
	}
	return true;
}