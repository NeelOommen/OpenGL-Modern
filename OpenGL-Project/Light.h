#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include "ShadowMap.h"

#include <glm/gtc/matrix_transform.hpp>

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, 
		GLfloat aIntensity, GLfloat dIntensity,
		GLuint shadowMapWidth, GLuint shadowMapHeight);

	ShadowMap* getShadowMap();

	~Light();

protected:
	//ambient
	glm::vec3 lightColour;
	GLfloat ambientIntensity;

	//diffuse
	GLfloat diffuseIntensity;

	//shadow
	ShadowMap* shadowMap;

	glm::mat4 lightProj;
};

