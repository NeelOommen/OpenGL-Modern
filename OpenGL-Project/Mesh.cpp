#include "Mesh.h"

Mesh::Mesh() {
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;
}

Mesh::~Mesh() {
	clearMesh();
}

void Mesh::clearMesh() {
	if (VBO != 0) {
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}

	if (IBO != 0) {
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}

	if (VAO != 0) {
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}

	indexCount = 0;
}

void Mesh::createMesh(GLfloat* vertices, unsigned int* indices, unsigned int numVertices, unsigned int numIndices) {
	//set index count
	indexCount = numIndices;
	
	//generate vertex array object
	glGenVertexArrays(1, &VAO);
	//bind the vao
	glBindVertexArray(VAO);

	//generate index buffer object
	glGenBuffers(1, &IBO);
	//bind ibo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	//fill ibo with data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numIndices, indices, GL_STATIC_DRAW);

	//generate vertex buffer object
	glGenBuffers(1, &VBO);
	//bind vbo
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//fill vbo with data
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numVertices, vertices, GL_STATIC_DRAW);

	//define the attribute pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//enable the vertext attrib pointer
	glEnableVertexAttribArray(0);

	//unbind all buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::renderMesh() {
	//bind the vao and ibo
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	//make the draw call
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

	//unbind buffers
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}