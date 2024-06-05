#pragma once
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

#include "CommonValues.h"

class Shader
{
public:
	Shader();
	~Shader();

	void createFromString(const char* vertexCode, const char* fragmentCode);
	void createFromFile(const char* vertexLocation, const char* fragmentLocation);

	GLuint getModelLocation();
	GLuint getProjectionLocation();
	GLuint getViewLocation();
	GLuint getAmbientIntensityLocation();
	GLuint getAmbientColourLocation();
	GLuint getDiffuseDirectionLocation();
	GLuint getDiffuseIntensityLocation();
	GLuint getSpecularIntensityLocation();
	GLuint getSpecularShininessLocation();
	GLuint getEyePositionLocation();

	void setDirectionalLight(DirectionalLight* dLight);
	void setPointLights(PointLight* pLights, unsigned int lightCount);
	void setSpotLights(SpotLight* sLights, unsigned int spotLightCount);

	void useShader();
	void clearShader();


private:
	int pointLightCount;
	int spotLightCount;

	GLuint shaderID, uniformModel, uniformProjection, uniformView;
	GLuint uniformSpecularIntensity, uniformSpecularShininess;
	GLuint uniformEyePosition;

	GLuint uniformPointLightCount;
	GLuint uniformSpotLightCount;

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDirection;
		GLuint uniformDiffuseIntensity;
	} uniformDirectionalLight;

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	} uniformPointLight[MAX_POINT_LIGHTS];

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformDirection;

		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;

		GLuint uniformEdge;
	} uniformSpotLight[MAX_SPOT_LIGHTS];

	void compileShader(const char*vertexCode, const char* fragmentShader);
	void addShader(GLuint id, const char* shaderCode, GLenum shaderType);

	std::string readFile(const char* fileLoc);
};

