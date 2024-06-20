#pragma once
#include "Light.h"
class DirectionalLight :
    public Light
{
public:
    DirectionalLight();
    DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity,
        GLfloat xDir, GLfloat yDir, GLfloat zDir,
        GLuint shadowMapWidth, GLuint shadowMapHeight);
    ~DirectionalLight();

    glm::mat4 calculateLightTransform();

    void useLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
        GLuint diffuseIntensityLocation, GLuint diffuseDirectionLocation);
private:
    glm::vec3 direction;
};

