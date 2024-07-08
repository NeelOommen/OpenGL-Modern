#pragma once
#include "ShadowMap.h"
class OmniShadowMap :
    public ShadowMap
{
public:
    OmniShadowMap();
    ~OmniShadowMap();

    bool init(GLuint width, GLuint height);
    void write();
    void read(GLenum textureUnit);

private:

};

