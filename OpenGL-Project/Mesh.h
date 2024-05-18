#pragma once
#include <GL/glew.h>

class Mesh
{
public:
	Mesh();
	~Mesh();

	void createMesh(GLfloat* vertices, unsigned int* indices, unsigned int numVertices, unsigned int numIndices);
	void renderMesh();
	void clearMesh();


private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;
};

