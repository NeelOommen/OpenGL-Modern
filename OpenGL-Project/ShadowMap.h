#pragma once

#include <stdio.h>
#include <GL/glew.h>

class ShadowMap
{
public:
	ShadowMap();

	virtual bool init(GLuint width, GLuint height);
	virtual void write();
	virtual void read(GLenum textureUnit);

	GLuint getWidth();
	GLuint getHeight();

	~ShadowMap();

protected:
	GLuint fbo;
	GLuint shadowMapID;
	GLuint shadowWidth, shadowHeight;
};

