#ifndef SPHERE_H
#define SPHERE_H
#include "../GL/glew.h"
#include <vector>
#include "../glm/glm.hpp"


class CShader;

class Sphere
{
private:
	unsigned int m_vaoID;		    // vertex array object
	unsigned int m_vboID[3];		// two VBOs - used for colours, vertex and normals data
	GLuint ibo;                     //identifier for the triangle indices

	int numOfVerts;
	int numOfTris;

	float* verts;
	float* norms;
	unsigned int* tInds;
	glm::vec3 centre = glm::vec3(0.0f, 0.0f, 0.0f);
	

	float cx,cy,cz; //centre of the sphere
	float r;		//radius of the sphere
public:
	Sphere();
	void constructGeometry(CShader* myShader, int level);
	void render();
	void setRadius(float rad);
	void setCentre(float x, float y, float z);
	glm::vec3 getCentre();
	float getRadius();
};
#endif