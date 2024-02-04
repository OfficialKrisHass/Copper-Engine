#version 330 core

struct Light {

	int type;
	
	vec3 position;
	vec3 direction;

	vec3 color;
	float intensity;
	
};

in vec3 a_position;
in vec3 a_color;
in vec3 a_normal;

uniform vec3 ambientDirection;
uniform vec3 ambientColor;

uniform Light light;
uniform vec3 camPos;

out vec4 FragColor;

void main() {

	vec3 normal = normalize(a_normal);
	vec3 viewDir = normalize(camPos - a_position);

	vec3 lightColor = light.color * light.intensity;
	vec3 lightDir;

	if (light.type == 0)
		lightDir = normalize(a_position - light.position);
	else
		lightDir = normalize(light.direction);

	// Ambient

	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor;

	// Diffuse
	
	vec3 diffuse = max(dot(normal, lightDir), 0.0f) * lightColor;
	vec3 ambientDirectional = max(dot(normal, normalize(ambientDirection)), 0.0f) * ambientColor * ambientStrength;

	// Specular

	float specularStrength = 0.5f;
	vec3 reflectDir = reflect(lightDir, normal);
	vec3 specular = pow(max(dot(viewDir, reflectDir), 0.0f), 32) * specularStrength * lightColor;
	
	vec3 final = (ambient + ambientDirectional + diffuse + specular) * a_color;
	FragColor = vec4(final, 1.0f);

}