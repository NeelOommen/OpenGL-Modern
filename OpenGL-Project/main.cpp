#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Texture.h"
#include "Light.h"
#include "Material.h"

//Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.1459265f / 180.0f;

std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

Texture brick;
Texture dirt;

Light ambient;
Material mat1;

//Vertex Shader
static const char* vShader = "Shaders/vertex.shader";

//Fragment Shader
static const char* fShader = "Shaders/fragment.shader";

void calcNormals(unsigned int* indices, unsigned int indicesCnt, GLfloat* vertices, unsigned int vertexCnt, unsigned int vLength, unsigned int normalOffset) {
	for (int i = 0; i < indicesCnt; i += 3) {
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i+1] * vLength;
		unsigned int in2 = indices[i+2] * vLength;

		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset;
		in1 += normalOffset;
		in2 += normalOffset;

		vertices[in0] += normal.x;
		vertices[in0 + 1] += normal.y;
		vertices[in0 + 2] += normal.z;

		vertices[in1] += normal.x;
		vertices[in1 + 1] += normal.y;
		vertices[in1 + 2] += normal.z;

		vertices[in2] += normal.x;
		vertices[in2 + 1] += normal.y;
		vertices[in2 + 2] += normal.z;
	}

	for (int i = 0; i < vertexCnt/vLength; i++) {
		unsigned nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x;
		vertices[nOffset + 1] = vec.y;
		vertices[nOffset + 2] = vec.z;
	}
}

void createTriangle() {
	GLfloat vertices[] = {
//		x		y		z		u		v		nX		nY		nZ	
		-1.0f,	-1.0f,	-0.6f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,
		0.0f,	-1.0f,	1.0f,	0.5f,	0.0f,	0.0f,	0.0f,	0.0f,
		1.0f,	-1.0f,	-0.6f,	1.0f,	0.0f,	0.0f,	0.0f,	0.0f,
		0.0f,	1.0f,	0.0f,	0.5f,	1.0f,	0.0f,	0.0f,	0.0f
	};

	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	calcNormals(indices, 12, vertices, 32, 8, 5);

	Mesh* triangle = new Mesh();
	triangle->createMesh(vertices, indices, 32, 12);
	meshList.push_back(triangle);
}

void createShader() {
	Shader* s1 = new Shader();
	s1->createFromFile(vShader, fShader);
	shaderList.push_back(s1);
}

int main() {
	Window window(WIDTH, HEIGHT);
	//window = glfwWindow(WIDTH, HEIGHT);
	window.initialize();

	Camera camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 1.0f);

	createTriangle();
	createShader();
	brick = Texture((char*)"Assets/Textures/brick.png");
	brick.loadTexture();

	dirt = Texture((char*)"Assets/Textures/dirt.png");
	dirt.loadTexture();

	ambient = Light(1.0f, 1.0f, 1.0f, 0.2f, 2.0f, -1.0f, -2.0f, 1.0f);

	//shiny
	mat1 = Material(1.0f, 32);
	//dull
	//mat1 = Material(0.3f, 4);

	int bufferWidth = window.getBufferWidth(), bufferHeight = window.getBufferHeight();

	glm::mat4 projectionMat = glm::perspective(45.0f, (GLfloat)bufferWidth / (GLfloat)bufferHeight, 0.1f, 100.0f);

	GLuint uniformModel, uniformProjection, uniformView;
	GLuint uniformAmbientIntensity, uniformAmbientColour;
	GLuint uniformDiffuseDirection, uniformDiffuseIntensity;
	GLuint uniformSpecIntensity, uniformSpecShininess;
	GLuint uniformEyePos;

	//Main Loop
	while (!window.shouldClose()) {
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		glfwPollEvents();

		camera.keyControl(window.getKeys(), deltaTime);
		camera.mouseControl(window.getXchange(), window.getYchange());

		//clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0]->useShader();
		uniformModel = shaderList[0]->getModelLocation();
		uniformProjection = shaderList[0]->getProjectionLocation();
		uniformView = shaderList[0]->getViewLocation();
		uniformAmbientIntensity = shaderList[0]->getAmbientIntensityLocation();
		uniformAmbientColour = shaderList[0]->getAmbientColourLocation();
		uniformDiffuseDirection = shaderList[0]->getDiffuseDirectionLocation();
		uniformDiffuseIntensity = shaderList[0]->getDiffuseIntensityLocation();
		uniformSpecIntensity = shaderList[0]->getSpecularIntensityLocation();
		uniformSpecShininess = shaderList[0]->getSpecularShininessLocation();
		uniformEyePos = shaderList[0]->getEyePositionLocation();

		ambient.useLight(uniformAmbientIntensity, uniformAmbientColour, uniformDiffuseIntensity, uniformDiffuseDirection);

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMat));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glm::vec3 cameraPos = camera.getCameraPositon();
		glUniform3f(uniformEyePos, cameraPos.x, cameraPos.y, cameraPos.z);


		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::rotate(model, 0.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brick.useTexture();
		mat1.useMaterial(uniformSpecIntensity, uniformSpecShininess);
		meshList[0]->renderMesh();
		
		glUseProgram(0);

		window.swapBuffers();
	}

	return 0;
}