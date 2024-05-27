#include "Material.h"

Material::Material() {
	specularIntensity = 0.0f;
	shininess = 0.0f;
}

Material::Material(GLfloat sIntensity, GLfloat shine) {
	specularIntensity = sIntensity;
	shininess = shine;
}

void Material::useMaterial(GLuint sIntensityLocation, GLuint shineLocation) {
	glUniform1f(sIntensityLocation, specularIntensity);
	glUniform1f(shineLocation, shininess);
}

Material::~Material() {

}