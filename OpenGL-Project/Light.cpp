#include "Light.h"

Light::Light() {
	lightColour = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;

	diffuseIntensity = 0.0f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity, GLuint shadowMapWidth, GLuint shadowMapHeight) {
	shadowMap = new ShadowMap();
	shadowMap->init(shadowMapWidth, shadowMapHeight);

	lightColour = glm::vec3(red, green, blue);
	ambientIntensity = aIntensity;

	diffuseIntensity = dIntensity;
}

ShadowMap* Light::getShadowMap() {
	return shadowMap;
}

Light::~Light() {

}