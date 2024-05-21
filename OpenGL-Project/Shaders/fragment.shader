#version 330

in vec4 vCol;
in vec2 texCoord;

out vec4 colour;

uniform sampler2D textureSampler;

void main()	{
	colour = texture(textureSampler, texCoord);
}
