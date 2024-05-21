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

//Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.1459265f / 180.0f;

std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

Texture brick;
Texture dirt;

//Vertex Shader
static const char* vShader = "Shaders/vertex.shader";

//Fragment Shader
static const char* fShader = "Shaders/fragment.shader";

void createTriangle() {
	GLfloat vertices[] = {
//		x		y		z		u		v
		-1.0f,	-1.0f,	0.0f,	0.0f,	0.0f,
		0.0f,	-1.0f,	1.0f,	0.5f,	0.0f,
		1.0f,	-1.0f,	0.0f,	1.0f,	0.0f,
		0.0f,	1.0f,	0.0f,	0.5f,	1.0f
	};

	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	Mesh* triangle = new Mesh();
	triangle->createMesh(vertices, indices, 20, 12);
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

	int bufferWidth = window.getBufferWidth(), bufferHeight = window.getBufferHeight();

	glm::mat4 projectionMat = glm::perspective(45.0f, (GLfloat)bufferWidth / (GLfloat)bufferHeight, 0.1f, 100.0f);

	GLuint uniformModel, uniformProjection, uniformView;

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

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::rotate(model, 0.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMat));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		brick.useTexture();
		meshList[0]->renderMesh();
		
		glUseProgram(0);

		window.swapBuffers();
	}

	return 0;
}