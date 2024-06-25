#version 330

in vec4 vCol;
in vec2 texCoord;
in vec3 norm;
in vec3 fragPos;
in vec4 directionalLightSpacePos;

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
uniform sampler2D directionalShadowMap;
uniform Material material;
uniform vec3 eyePos;

float calcDirectionalShadowFactor(DirectionalLight light){
	vec3 projCoords = directionalLightSpacePos.xyz / directionalLightSpacePos.w;
	projCoords = (projCoords * 0.5) + 0.5;

	float current = projCoords.z;

	vec3 n = normalize(norm);
	vec3 lightDir = normalize(directionLight.direction);

	float bias = max(0.05 * (1 - dot(n, lightDir)), 0.005);

	float shadow = 0.0;

	vec2 texelSize = 1.0 / textureSize(directionalShadowMap, 0);
	for(int i = -1; i<=1; i++){
		for(int j = -1; j<=1; j++){
			float pcfDepth = texture(directionalShadowMap, projCoords.xy + vec2(i,j) * texelSize).r;
			shadow += current - bias > pcfDepth ? 1.0: 0.0;
		}
	} 

	shadow /= 9.0;

	if(projCoords.z > 1.0){
		shadow = 0.0;
	}

	return shadow;

}

vec4 calcLightByDirection(Light light, vec3 direction, float shadowFactor){
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

	return (ambientColour + (1.0f - shadowFactor) * (diffuseColour + specColour));
}

vec4 calcDirectionalLight(){
	float shadowFactor = calcDirectionalShadowFactor(directionLight);
	return calcLightByDirection(directionLight.base, directionLight.direction, shadowFactor);
}

vec4 calcPointLight(PointLight pLight){
	vec3 direction = fragPos - pLight.position;
	float distance = length(direction);
	direction = normalize(direction);
	vec4 col = calcLightByDirection(pLight.base, direction, 0.0f);
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
