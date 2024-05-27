#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,
		GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity);
	~Light();

	void useLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
		GLuint diffuseIntensityLocation, GLuint diffuseDirectionLocation);
private:
	//ambient
	glm::vec3 lightColour;
	GLfloat ambientIntensity;

	//diffuse
	glm::vec3 direction;
	GLfloat diffuseIntensity;
};

