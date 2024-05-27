#pragma once
#include <GL/glew.h>

class Material
{
public:
	Material();
	Material(GLfloat sIntensity, GLfloat shine);
	void useMaterial(GLuint sIntensityLocation, GLuint shineLocation);
	~Material();

private:
	GLfloat specularIntensity;
	GLfloat shininess;
};

