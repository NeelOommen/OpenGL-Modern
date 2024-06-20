#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light() {
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	lightProj = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 20.0f);
}

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, 
							GLfloat aIntensity, GLfloat dIntensity,
							GLfloat xDir, GLfloat yDir, GLfloat zDir,
							GLuint shadowMapWidth, GLuint shadowMapHeight) :	Light(red, green, blue, aIntensity, dIntensity, shadowMapWidth, shadowMapHeight) {
	direction = glm::vec3(xDir, yDir, zDir);
	lightProj = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 20.0f);
}

void DirectionalLight::useLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
	GLuint diffuseIntensityLocation, GLuint diffuseDirectionLocation) {
	glUniform3f(ambientColourLocation, lightColour.x, lightColour.y, lightColour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);

	glUniform3f(diffuseDirectionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}

glm::mat4 DirectionalLight::calculateLightTransform() {
	return lightProj * glm::lookAt(-direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

DirectionalLight::~DirectionalLight() {

}