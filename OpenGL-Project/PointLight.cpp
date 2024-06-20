#include "PointLight.h"

PointLight::PointLight(): Light()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	constant = 1.0f;
	linear = 0.0f;
	exponent = 0.0f;
}

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity, GLfloat x, GLfloat y, GLfloat z, GLfloat cons, GLfloat lin, GLfloat exp):
	Light(red, green, blue, aIntensity, dIntensity, 1024, 1024)
{
	position = glm::vec3(x, y, z);
	constant = cons;
	linear = lin;
	exponent = exp;
}

PointLight::~PointLight()
{
}

void PointLight::useLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation, GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation)
{
	//ambient uniforms
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform3f(ambientColourLocation, lightColour.x, lightColour.y, lightColour.z);

	//diffuse uniforms
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
	
	//light position uniform
	glUniform3f(positionLocation, position.x, position.y, position.z);

	//attenuations uniforms
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);
}
