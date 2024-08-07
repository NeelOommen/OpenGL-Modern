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

#include "CommonValues.h"

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include "Model.h"

#include <assimp/Importer.hpp>

//Window dimensions
const GLint WIDTH = 1280, HEIGHT = 720;
const float toRadians = 3.1459265f / 180.0f;

std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;

Shader directionalShadowShader;
Shader omniShadowShader;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

Texture brick;
Texture dirt;

DirectionalLight ambient;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
Material mat1;
Material mat2;

Model xwing;

GLuint uniformModel, uniformProjection, uniformView;
GLuint uniformSpecIntensity, uniformSpecShininess;
GLuint uniformEyePos;
GLuint uniformOmniLightPos, uniformFarPlane;

Camera camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 1.0f);

unsigned int pointLightCount = 0;
unsigned int spotLightCount = 0;

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

	GLfloat floorVertices[] = {
		-10.0f,	0.0f,	-10.0f,	0.0f,	0.0f,	0.0f,	-1.0f, 0.0f,
		10.0f,	0.0f,	-10.0f, 10.0f,	0.0f,	0.0f,	-1.0f, 0.0f,
		-10.0f, 0.0f,	10.0f,	0.0f,	10.0f,	0.0f,	-1.0f, 0.0f,
		10.0f,	0.0f,	10.0f,	10.0f,	10.0f,	0.0f,	-1.0f, 0.0f,
	};

	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	unsigned int floorIndices[] = {
		0,2,1,
		1,2,3
	};

	calcNormals(indices, 12, vertices, 32, 8, 5);

	Mesh* triangle = new Mesh();
	triangle->createMesh(vertices, indices, 32, 12);
	meshList.push_back(triangle);

	Mesh* floorMesh = new Mesh();
	floorMesh->createMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(floorMesh);
}

void createShader() {
	Shader* s1 = new Shader();
	s1->createFromFile(vShader, fShader);
	shaderList.push_back(s1);

	//directionalShadowShader = Shader();
	//omniShadowShader = Shader();

	directionalShadowShader.createFromFile("Shaders/directional_shadow_vert.shader", "Shaders/directional_shadow_frag.shader");
	omniShadowShader.createFromFile("Shaders/omni_vert.shader", "Shaders/omni_geometry.shader", "Shaders/omni_frag.shader");
}

void renderTestScene() {
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
	model = glm::rotate(model, 0.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	brick.useTexture();
	mat1.useMaterial(uniformSpecIntensity, uniformSpecShininess);
	meshList[0]->renderMesh();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 2.0f, 2.0f));
	model = glm::rotate(model, 0.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	xwing.renderModel();

	glm::mat4 floorModel(1.0f);
	floorModel = glm::translate(floorModel, glm::vec3(0.0f, -2.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(floorModel));
	dirt.useTexture();
	mat2.useMaterial(uniformSpecIntensity, uniformSpecShininess);
	meshList[1]->renderMesh();
}

void shadowMapPass(DirectionalLight* light) {
	directionalShadowShader.useShader();

	glViewport(0, 0, light->getShadowMap()->getWidth(), light->getShadowMap()->getHeight());

	light->getShadowMap()->write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = directionalShadowShader.getModelLocation();
	glm::mat4 lightTransform = light->calculateLightTransform();
	directionalShadowShader.setDirectionalLightTransform(&lightTransform);

	directionalShadowShader.validate();

	renderTestScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void omniShadowPass(PointLight* light) {
	omniShadowShader.useShader();

	glViewport(0, 0, light->getShadowMap()->getWidth(), light->getShadowMap()->getHeight());

	light->getShadowMap()->write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = omniShadowShader.getModelLocation();

	uniformOmniLightPos = omniShadowShader.getLightPosLocation();
	uniformFarPlane = omniShadowShader.getFarPlaneLocation();

	glUniform3f(uniformOmniLightPos, light->getPosition().x, light->getPosition().y, light->getPosition().z);
	glUniform1f(uniformFarPlane, light->getFarPlane());

	omniShadowShader.setLightMatrices(light->calculateLightTransforms());

	omniShadowShader.validate();

	renderTestScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void renderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix) {
	Shader* sh = shaderList[0];
	sh->useShader();
	sh->validate();

	uniformModel = sh->getModelLocation();
	uniformProjection = sh->getProjectionLocation();
	uniformView = shaderList[0]->getViewLocation();
	uniformSpecIntensity = sh->getSpecularIntensityLocation();
	uniformSpecShininess = sh->getSpecularShininessLocation();
	uniformEyePos = sh->getEyePositionLocation();

	glViewport(0, 0, WIDTH, HEIGHT);

	//clear window
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	spotLights[1].setFlash(camera.getCameraPositon(), camera.getCameraDirection());

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glm::vec3 cameraPos = camera.getCameraPositon();
	glUniform3f(uniformEyePos, cameraPos.x, cameraPos.y, cameraPos.z);

	ambient.getShadowMap()->read(GL_TEXTURE2);
	//set Directional Light
	sh->setDirectionalLight(&ambient);
	//set Point Lights
	sh->setPointLights(pointLights, pointLightCount, 3, 0);
	//Set Spot Lights
	sh->setSpotLights(spotLights, spotLightCount, 3 + pointLightCount, pointLightCount);

	glm::mat4 ambientTransform = ambient.calculateLightTransform();
	shaderList[0]->setDirectionalLightTransform(&ambientTransform);
	shaderList[0]->setTexture(1);
	shaderList[0]->setDirectionalShadowMap(2);

	shaderList[0]->validate();

	renderTestScene();
}

int main() {
	Window window(WIDTH, HEIGHT);
	//window = glfwWindow(WIDTH, HEIGHT);
	window.initialize();

	createTriangle();
	createShader();
	brick = Texture((char*)"Assets/Textures/brick.png");
	brick.loadTextureWithAlpha();

	dirt = Texture((char*)"Assets/Textures/dirt.png");
	dirt.loadTextureWithAlpha();

	ambient = DirectionalLight(1.0f, 1.0f, 1.0f, //r g b
								0.1f, 0.6f, //aIntensity dIntensity
								0.0f, -7.0f, -1.0f, // x y z
								1024, 1024 //shadowmap dimensions
								); 


	pointLights[0] = PointLight(1024, 1024, //shadow map resolution
		0.01f, 100.0f, //near and far plane
		0.0f, 1.0f, 0.0f, //r g b
		0.1f, 0.1f,	//aIntensity dIntensity
		-4.0f, 2.0f, 0.0f, //x y z
		0.3f, 0.2f, 0.1f); //constant linear exponent

	pointLightCount++;

	pointLights[1] = PointLight( 1024, 1024, //shadow map resolution
		0.01f, 100.0f, //near and far plane
		1.0f, 0.0f, 1.0f, //r g b
		0.1f, 0.1f,	//aIntensity dIntensity
		4.0f, 1.0f, 0.0f, //x y z
		0.3f, 0.2f, 0.1f); //constant linear exponent

	pointLightCount++;


	spotLights[0] = SpotLight(1024, 1024, //shadow map resolution
		0.01f, 100.0f, //near and far plane
		1.0f, 1.0f, 0.0f, //r g b
		0.1f, 1.0f,	//aIntensity dIntensity
		-4.0f, 2.0f, 0.0f, //x y z
		0.0f, -1.0f, 0.0f, //direction
		0.3f, 0.2f, 0.1f, //constant linear exponent
		20.0f); //edge

	spotLightCount++;

	spotLights[1] = SpotLight(1024, 1024, //shadow map resolution
		0.01f, 100.0f, //near and far plane
		1.0f, 1.0f, 1.0f, //r g b
		0.1f, 2.0f,	//aIntensity dIntensity
		-4.0f, 2.0f, 0.0f, //x y z
		0.0f, -1.0f, 0.0f, //direction
		1.0f, 0.0f, 0.0f, //constant linear exponent
		20.0f); //edge

	spotLightCount++;

	//shiny
	mat1 = Material(2.0f, 8);
	//dull
	mat2 = Material(1.0f, 4);

	xwing = Model();
	xwing.loadModel("Assets/Models/Seahawk.obj");

	int bufferWidth = window.getBufferWidth(), bufferHeight = window.getBufferHeight();

	glm::mat4 projectionMat = glm::perspective(glm::radians(60.0f), (GLfloat)bufferWidth / (GLfloat)bufferHeight, 0.1f, 100.0f);

	//Main Loop
	while (!window.shouldClose()) {
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		glfwPollEvents();

		camera.keyControl(window.getKeys(), deltaTime);
		camera.mouseControl(window.getXchange(), window.getYchange());

		shadowMapPass(&ambient);

		for (size_t i = 0; i < pointLightCount;i++) {
			omniShadowPass(&pointLights[i]);
		}
		for (size_t i = 0; i < spotLightCount; i++) {
			omniShadowPass(&spotLights[i]);
		}

		renderPass(projectionMat, camera.calculateViewMatrix());
		
		glUseProgram(0);

		window.swapBuffers();
	}

	return 0;
}