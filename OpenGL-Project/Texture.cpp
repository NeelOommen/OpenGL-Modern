#include "Texture.h"

Texture::Texture()
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = (char*)"";
}

Texture::Texture(const char* fileLoc)
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = fileLoc;
}

Texture::~Texture()
{
	clearTexture();
}

bool Texture::loadTexture()
{
	unsigned char* textureData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);

	//error check
	if (!textureData) {
		printf("Could not load texture: %s", fileLocation);
		clearTexture();
		return false;
	}

	//generate buffer
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	//define parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 
		0, 
		GL_RGB,
		width,
		height,
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		textureData
		);
	glGenerateMipmap(GL_TEXTURE_2D);

	//unbind buffer
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(textureData);

	return true;
}

bool Texture::loadTextureWithAlpha()
{
	unsigned char* textureData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);

	//error check
	if (!textureData) {
		printf("Could not load texture: %s", fileLocation);
		clearTexture();
		return false;
	}

	//generate buffer
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	//define parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		width,
		height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		textureData
	);
	glGenerateMipmap(GL_TEXTURE_2D);

	//unbind buffer
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(textureData);

	return true;
}

void Texture::useTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::clearTexture()
{
	glDeleteTextures(1, &textureID);
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = (char*)"";
}
