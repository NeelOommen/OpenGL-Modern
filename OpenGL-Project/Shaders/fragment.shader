#version 330

in vec4 vCol;
in vec2 texCoord;
in vec3 norm;
in vec3 fragPos;

out vec4 colour;

struct DirectionalLight{
	vec3 colour;
	float ambientIntensity;
	vec3 direction;
	float diffuseIntensity;
};

struct Material{
	float specularIntensity;
	float shininess;
};

uniform DirectionalLight directionLight;
uniform sampler2D textureSampler;
uniform Material material;
uniform vec3 eyePos;


void main()	{
	vec4 ambientColour = vec4(directionLight.colour, 1.0f) * directionLight.ambientIntensity;

	float diffuseFactor = max(dot(normalize(norm), normalize(directionLight.direction)), 0.0f);
	vec4 diffuseColour = vec4(directionLight.colour, 1.0f)  * directionLight.diffuseIntensity * diffuseFactor;

	vec4 specColour = vec4(0,0,0,0);

	if(diffuseFactor > 0.0f){
		vec3 fragToEye = normalize(eyePos - fragPos);
		vec3 reflVertex = normalize(reflect(directionLight.direction, normalize(norm)));

		float specFactor = dot(fragToEye, reflVertex);
		if(specFactor > 0.0f){
			specFactor = pow(specFactor, material.shininess);
			specColour = vec4(directionLight.colour * material.specularIntensity * specFactor, 1.0f);
		}
	}

	colour = texture(textureSampler, texCoord) * (ambientColour + diffuseColour + specColour);
}
