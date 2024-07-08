#pragma once
#include "Light.h"
#include "OmniShadowMap.h"
#include <vector>

class PointLight :
    public Light
{
public:
    PointLight();
    PointLight( GLuint shadowWidth, GLuint shadowHeight,
                GLfloat near, GLfloat far,
                GLfloat red, GLfloat green, GLfloat blue,
                GLfloat aIntensity, GLfloat dIntensity,
                GLfloat x, GLfloat y, GLfloat z,
                GLfloat cons, GLfloat lin, GLfloat exp);
    ~PointLight();

    void useLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
        GLuint diffuseIntensityLocation, GLuint positionLocation,
        GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);

    std::vector<glm::mat4> calculateLightTransforms();
    GLfloat getFarPlane();
    glm::vec3 getPosition();

protected:
    glm::vec3 position;

    GLfloat constant, linear, exponent;

    GLfloat farPlane;
};

