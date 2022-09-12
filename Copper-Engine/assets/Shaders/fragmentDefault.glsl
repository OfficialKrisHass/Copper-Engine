#version 330 core

in vec3 a_position;
in vec3 a_color;
in vec3 a_normal;

uniform vec3 lightPos;
uniform vec3 camPos;

out vec4 FragColor;

void main() {
	
	//Ambient Light
	vec3 ambient = vec3(0.1f, 0.1f, 0.1f);
	
	//Diffuse Light
	vec3 posToLightDirVec = normalize(a_position - lightPos);
	vec3 diffuseColor = vec3(1.0f, 1.0f, 1.0f);
	float diffuse = clamp(dot(posToLightDirVec, a_normal), 0.0f, 1.0f);
	vec3 diffuseFinal = diffuseColor * diffuse;
	
	//Specular Light
	vec3 lightToPosDirVec = normalize(lightPos - a_position);
	vec3 reflectDirVec = normalize(reflect(lightToPosDirVec, normalize(a_normal)));
	vec3 posToViewDirVec = normalize(a_position - camPos);
	float specularConstant = pow(max(dot(posToViewDirVec, reflectDirVec), 0.0f), 50.0f);
	vec3 specularFinal = vec3(1.0f, 1.0f, 1.0f) * specularConstant;
	
	//Final Fragment Color
	FragColor = vec4(a_color, 1.0) * (vec4(ambient, 1.0f) + vec4(diffuseFinal, 1.0f) + vec4(specularFinal, 1.0f));

}