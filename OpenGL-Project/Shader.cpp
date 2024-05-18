#include "Shader.h"

Shader::Shader() {
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;
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