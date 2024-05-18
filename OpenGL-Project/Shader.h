#pragma once
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>

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

	void useShader();
	void clearShader();


private:
	GLuint shaderID, uniformModel, uniformProjection, uniformView;

	void compileShader(const char*vertexCode, const char* fragmentShader);
	void addShader(GLuint id, const char* shaderCode, GLenum shaderType);

	std::string readFile(const char* fileLoc);
};

