#include "Shader.h"

Shader::Shader() {
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;

	pointLightCount = 0;
	spotLightCount = 0;
}

void Shader::compileShader(const char* vertexCode, const char* fragmentCode) {
	//get a new shader id
	shaderID = glCreateProgram();

	if (!shaderID) {
		printf("Failed to create Shader");
		return;
	}

	//create the individual shaders
	addShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	addShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar errorLog[1024] = { 0 };

	//link shader
	glLinkProgram(shaderID);

	//error check for linking
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderID, sizeof(errorLog), NULL, errorLog);
		printf("Shader Compilation Error : % s", errorLog);
		return;
	}

	//validate shader
	glValidateProgram(shaderID);

	//error check for validation
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderID, sizeof(errorLog), NULL, errorLog);
		printf("Shader Compilation Error: %s", errorLog);
		return;
	}

	//store the handles for the model and projection uniforms
	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformView = glGetUniformLocation(shaderID, "view");
	uniformDirectionalLight.uniformColour = glGetUniformLocation(shaderID, "directionLight.base.colour");
	uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionLight.base.ambientIntensity");
	uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "directionLight.direction");
	uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionLight.base.diffuseIntensity");
	uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
	uniformSpecularShininess = glGetUniformLocation(shaderID, "material.shininess");
	uniformEyePosition = glGetUniformLocation(shaderID, "eyePos");

	uniformTexture = glGetUniformLocation(shaderID, "textureSampler");
	uniformDirectionalLightTransform = glGetUniformLocation(shaderID, "directionalLightTransform");
	uniformDirectionalShadowMap = glGetUniformLocation(shaderID, "directionalShadowMap");

	//point light
	uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");

	for (size_t i = 0; i < MAX_POINT_LIGHTS; i++) {
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.colour", i);
		uniformPointLight[i].uniformColour = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", i);
		uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
		uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i);
		uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", i);
		uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", i);
		uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", i);
		uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);
	}

	//spotlight
	uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");

	for (size_t i = 0; i< MAX_SPOT_LIGHTS; i++) {
		char locBuff[100] = {'\0'};

		//base light class
		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.colour", i);
		uniformSpotLight[i].uniformColour = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.ambientIntensity", i);
		uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.diffuseIntensity", i);
		uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

		//point light
		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.position", i);
		uniformSpotLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.constant", i);
		uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.linear", i);
		uniformSpotLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.exponent", i);
		uniformSpotLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);

		//only for spotlight
		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].direction", i);
		uniformSpotLight[i].uniformDirection = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].edge", i);
		uniformSpotLight[i].uniformEdge = glGetUniformLocation(shaderID, locBuff);
	}
}

void Shader::addShader(GLuint id, const char* shaderCode, GLenum shaderType) {
	//get handle for the shader to be created
	GLuint shader = glCreateShader(shaderType);

	//format the code in required arrays and dtypes
	const GLchar* shaderSource[1];
	shaderSource[0] = shaderCode;
	GLint sourceLen[1];
	sourceLen[0] = strlen(shaderCode);

	//set the shader code
	glShaderSource(shader, 1, shaderSource, sourceLen);
	//attempt to compile
	glCompileShader(shader);

	//error check the compilation
	GLint result = 0;
	GLchar errorLog[1024] = { 0 };

	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(shader, sizeof(errorLog), NULL, errorLog);
		printf("Shader %d failed to compile: %s", shaderType, errorLog);
		return;
	}

	//attach the shader to the overall program
	glAttachShader(id, shader);
}

void Shader::useShader() {
	glUseProgram(shaderID);
}

GLuint Shader::getModelLocation() {
	return uniformModel;
}

GLuint Shader::getProjectionLocation() {
	return uniformProjection;
}

GLuint Shader::getViewLocation() {
	return uniformView;
}

GLuint Shader::getAmbientColourLocation() {
	return uniformDirectionalLight.uniformColour;
}

GLuint Shader::getAmbientIntensityLocation() {
	return uniformDirectionalLight.uniformAmbientIntensity;
}

GLuint Shader::getDiffuseDirectionLocation() {
	return uniformDirectionalLight.uniformDirection;
}

GLuint Shader::getDiffuseIntensityLocation() {
	return uniformDirectionalLight.uniformDiffuseIntensity;
}

GLuint Shader::getSpecularIntensityLocation() {
	return uniformSpecularIntensity;
}

GLuint Shader::getSpecularShininessLocation() {
	return uniformSpecularShininess;
}

GLuint Shader::getEyePositionLocation() {
	return uniformEyePosition;
}

void Shader::createFromString(const char* vertexCode, const char* fragmentCode) {
	compileShader(vertexCode, fragmentCode);
}

void Shader::clearShader() {
	if (shaderID != 0) {
		glDeleteProgram(shaderID);
		shaderID = 0;
	}

	uniformModel = 0;
	uniformProjection = 0;
}

void Shader::setDirectionalLight(DirectionalLight* dLight) {
	dLight->useLight(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformColour, uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
}

void Shader::setPointLights(PointLight* pLights, unsigned int lightCount)
{
	if (lightCount > MAX_POINT_LIGHTS) {
		lightCount = MAX_POINT_LIGHTS;
	}

	glUniform1i(uniformPointLightCount, lightCount);

	for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
		pLights[i].useLight(uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformColour, uniformPointLight[i].uniformDiffuseIntensity,
			uniformPointLight[i].uniformPosition,
			uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformExponent);
	}
}

void Shader::setSpotLights(SpotLight* sLights, unsigned int spotLightCount)
{
	if (spotLightCount > MAX_SPOT_LIGHTS) {
		spotLightCount = MAX_SPOT_LIGHTS;
	}

	glUniform1i(uniformSpotLightCount, spotLightCount);

	for (int i = 0; i < spotLightCount; i++) {
		sLights[i].useLight(uniformSpotLight[i].uniformAmbientIntensity, uniformSpotLight[i].uniformColour, uniformSpotLight[i].uniformDiffuseIntensity,
			uniformSpotLight[i].uniformPosition, uniformSpotLight[i].uniformDirection, 
			uniformSpotLight[i].uniformConstant, uniformSpotLight[i].uniformLinear, uniformSpotLight[i].uniformExponent,
			uniformSpotLight[i].uniformEdge);
	}
}

void Shader::setTexture(GLint textureUnit) {
	glUniform1i(uniformTexture, textureUnit);
}

void Shader::setDirectionalShadowMap(GLint textureUnit) {
	glUniform1i(uniformDirectionalShadowMap, textureUnit);
}

void Shader::setDirectionalLightTransform(glm::mat4* lTransform) {
	glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(*lTransform));
}

Shader::~Shader() {
	clearShader();
}

std::string Shader::readFile(const char* location) {
	std::string shaderSource;
	//open the file
	std::ifstream fileStream(location, std::ios::in);

	//error check file
	if (!fileStream.is_open()) {
		printf("Failed to open %s", location);
		return "";
	}

	//read the file in line by line
	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		shaderSource.append(line+'\n');
	}

	return shaderSource;
}

void Shader::createFromFile(const char* vertexLocation, const char* fragmentLocation) {
	//read in the shaders
	std::string vString = readFile(vertexLocation);
	std::string fString = readFile(fragmentLocation);

	//convert to c style strings
	const char* vertexCode = vString.c_str();
	const char* fragmentCode = fString.c_str();

	//create shader from the strings
	createFromString(vertexCode, fragmentCode);
}