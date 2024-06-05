#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity);
	~Light();

protected:
	//ambient
	glm::vec3 lightColour;
	GLfloat ambientIntensity;

	//diffuse
	GLfloat diffuseIntensity;
};

