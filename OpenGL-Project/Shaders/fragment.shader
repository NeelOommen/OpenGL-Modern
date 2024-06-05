#version 330

in vec4 vCol;
in vec2 texCoord;
in vec3 norm;
in vec3 fragPos;

out vec4 colour;

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

struct Light{
	vec3 colour;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight{
	Light base;
	vec3 direction;
};

struct PointLight{
	Light base;
	vec3 position;
	float constant;
	float linear;
	float exponent;
};

struct SpotLight{
	PointLight base;
	vec3 direction;
	float edge;
};

struct Material{
	float specularIntensity;
	float shininess;
};

uniform int pointLightCount;
uniform int spotLightCount;

uniform DirectionalLight directionLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform sampler2D textureSampler;
uniform Material material;
uniform vec3 eyePos;

vec4 calcLightByDirection(Light light, vec3 direction){
	vec4 ambientColour = vec4(light.colour, 1.0f) * light.ambientIntensity;

	float diffuseFactor = max(dot(normalize(norm), normalize(direction)), 0.0f);
	vec4 diffuseColour = vec4(light.colour, 1.0f)  * light.diffuseIntensity * diffuseFactor;

	vec4 specColour = vec4(0,0,0,0);

	if(diffuseFactor > 0.0f){
		vec3 fragToEye = normalize(eyePos - fragPos);
		vec3 reflVertex = normalize(reflect(direction, normalize(norm)));

		float specFactor = dot(fragToEye, reflVertex);
		if(specFactor > 0.0f){
			specFactor = pow(specFactor, material.shininess);
			specColour = vec4(light.colour * material.specularIntensity * specFactor, 1.0f);
		}
	}

	return (ambientColour + diffuseColour + specColour);
}

vec4 calcDirectionalLight(){
	return calcLightByDirection(directionLight.base, directionLight.direction);
}

vec4 calcPointLight(PointLight pLight){
	vec3 direction = fragPos - pLight.position;
	float distance = length(direction);
	direction = normalize(direction);
	vec4 col = calcLightByDirection(pLight.base, direction);
	float attenuation = pLight.exponent * distance * distance +
						pLight.linear * distance + 
						pLight.constant;

	return (col / attenuation);
}


vec4 calcSpotLight(SpotLight sLight){
	vec3 rayDirection = normalize(fragPos - sLight.base.position);
	float slFactor = dot(rayDirection, sLight.direction);

	if(slFactor > sLight.edge){
		vec4 col = calcPointLight(sLight.base);

		return col * (1.0f - (1.0f - slFactor) * (1.0f / (1.0f - sLight.edge)));
	}
	
	return vec4(0,0,0,0);
}

vec4 calcPointLights(){
	vec4 totalColour = vec4(0,0,0,0);

	for(int i = 0; i < pointLightCount; i++){
		totalColour += calcPointLight(pointLights[i]);
	}

	return totalColour;
}

vec4 calcSpotLights(){
	vec4 totalColour = vec4(0,0,0,0);

	for(int i = 0; i < spotLightCount; i++){
		totalColour += calcSpotLight(spotLights[i]);
	}

	return totalColour;
}


void main()	{
	vec4 finalColour = calcDirectionalLight();	
	finalColour += calcPointLights();
	finalColour += calcSpotLights();

	colour = texture(textureSampler, texCoord) * finalColour;
}
