#pragma once
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
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

	void createFromFile(const char* vertexLocation, const char* fragmentLocation);
	void createFromFile(const char* vertexLocation, const char* geometryLocation, const char* fragmentLocation);

	void validate();

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
	GLuint getLightPosLocation();
	GLuint getFarPlaneLocation();

	void setDirectionalLight(DirectionalLight* dLight);
	void setPointLights(PointLight* pLights, unsigned int lightCount, unsigned int textureUnit, unsigned int offset);
	void setSpotLights(SpotLight* sLights, unsigned int spotLightCount, unsigned int textureUnit, unsigned int offset);

	void setTexture(GLint textureUnit);
	void setDirectionalShadowMap(GLint textureUnit);
	void setDirectionalLightTransform(glm::mat4* lTransform);
	void setLightMatrices(std::vector<glm::mat4> lightMatrices);

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

	GLuint uniformTexture;

	GLuint uniformDirectionalLightTransform, uniformDirectionalShadowMap;
	GLuint uniformOmniLightPos, uniformFarPlane;

	GLuint uniformLightMatrices[6];

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

	struct {
		GLuint shadowMap;
		GLuint farPlane;
	} uniformOmniShadowMaps[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];

	void compileShader(const char*vertexCode, const char* fragmentCode);
	void compileShader(const char*vertexCode, const char* geometryCode, const char* fragmentCode);
	void addShader(GLuint id, const char* shaderCode, GLenum shaderType);

	void compileProgram();

	std::string readFile(const char* fileLoc);
};

