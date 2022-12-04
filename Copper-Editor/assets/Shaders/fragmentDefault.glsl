#version 330 core

struct Light {
	
	vec3 position;
	vec3 color;
	float intensity;
	
};

in vec3 a_position;
in vec3 a_color;
in vec3 a_normal;

uniform Light light;
uniform vec3 camPos;

out vec4 FragColor;

void main() {
	
	//Ambient Light
	vec3 ambient = vec3(0.1f, 0.1f, 0.1f);
	
	//Diffuse Light
	vec3 diffuseColor = vec3(1.0f, 1.0f, 1.0f);
	vec3 posToLightDirVec = normalize(a_position - light.position);
	float diffuse = clamp(dot(posToLightDirVec, a_normal), 0.0f, 1.0f);
	vec3 diffuseFinal = diffuse * (diffuseColor * (light.color * light.intensity));
	
	//Specular Light
	vec3 lightToPosDirVec = normalize(light.position - a_position);
	vec3 reflectDirVec = normalize(reflect(lightToPosDirVec, normalize(a_normal)));
	vec3 posToViewDirVec = normalize(a_position - camPos);
	float specularConstant = pow(max(dot(posToViewDirVec, reflectDirVec), 0.0f), 100.0f);
	vec3 specularFinal = vec3(1.0f, 1.0f, 1.0f) * specularConstant;
	
	//Final Fragment Color
	FragColor = (vec4(diffuseColor, 1.0)) * (vec4(ambient, 1.0f) + vec4(diffuseFinal, 1.0f) + vec4(specularFinal, 1.0f));

}