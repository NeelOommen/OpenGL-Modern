#include "ShadowMap.h"

ShadowMap::ShadowMap() {
	fbo = 0;
	shadowMapID = 0;
	shadowWidth = 0;
	shadowHeight = 0;
}

ShadowMap::~ShadowMap() {
	if (fbo) {
		glDeleteFramebuffers(1, &fbo);
	}

	if (shadowMapID) {
		glDeleteTextures(1, &shadowMapID);
	}
}

GLuint ShadowMap::getWidth() {
	return shadowWidth;
}

GLuint ShadowMap::getHeight() {
	return shadowHeight;
}

bool ShadowMap::init(GLuint width, GLuint height) {
	shadowWidth = width;
	shadowHeight = height;

	glGenFramebuffers(1, &fbo);

	glGenTextures(1, &shadowMapID);
	glBindTexture(GL_TEXTURE_2D, shadowMapID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColour[] = {1.0f, 1.0f, 1.0f, 1.0f,};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColour);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapID, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE) {
		printf("Frame buffer error: %i\n", status);
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

void ShadowMap::write() {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void ShadowMap::read(GLint textureUnit) {
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, shadowMapID);
}