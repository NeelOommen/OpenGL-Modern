#pragma once
#include "PointLight.h"
class SpotLight :
    public PointLight
{
public:
    SpotLight();
    SpotLight(GLuint shadowWidth, GLuint shadowHeight, GLfloat near, GLfloat far, GLfloat red, GLfloat green, GLfloat blue,
        GLfloat aIntensity, GLfloat dIntensity,
        GLfloat x, GLfloat y, GLfloat z,
        GLfloat xDir, GLfloat yDir, GLfloat zDir,
        GLfloat cons, GLfloat lin, GLfloat exp,
        GLfloat edg);
    ~SpotLight();

    void useLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
        GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation,
        GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
        GLuint edgeLocation);

    void setFlash(glm::vec3 pos, glm::vec3 dir);
private:
    glm::vec3 direction;

    GLfloat edge, procEdge;
};

