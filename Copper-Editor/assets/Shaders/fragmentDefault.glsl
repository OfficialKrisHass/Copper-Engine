#version 330 core

#define POINT_LIGHT 0
#define DIRECTIONAL_LIGHT 1

#define MAX_LIGHTS 8
#define MAX_MATERIALS 16

struct Light {

	int type;
	vec3 posOrDir;

	vec3 color;
	float intensity;
	
};

struct Material {

	sampler2D texture;
	vec4 albedo;

};

in vec3 a_position;
in vec3 a_color;
in vec3 a_normal;
in vec2 a_uv;
flat in int a_matIndex;

uniform vec3 camPos;

// Lighting uniforms

uniform Light lights[MAX_LIGHTS];
uniform int lightCount;

uniform vec3 ambientDirection;
uniform vec3 ambientColor;

uniform float ambientStrength;
uniform float specularStrength;

// Material uniforms

uniform Material materials[MAX_MATERIALS];

out vec4 FragColor;

vec3 LightColor(vec3 direction, vec3 color, vec3 normal, vec3 viewDir);
vec3 AmbientLightColor(vec3 direction, vec3 color, vec3 normal);

void main() {

	vec3 normal = normalize(a_normal);
	vec3 viewDir = normalize(camPos - a_position);

	vec3 lightResult = vec3(0.0f);
	for (int i = 0; i < lightCount; i++) {

		Light light = lights[i];

		vec3 lightDir;
		if (light.type == POINT_LIGHT)
			lightDir = normalize(a_position - light.posOrDir);
		else
			lightDir = normalize(light.posOrDir);

		lightResult += LightColor(lightDir, light.color * light.intensity, normal, viewDir);

	}
	lightResult += AmbientLightColor(ambientDirection, ambientColor, normal);

	vec4 textureColor = texture(materials[a_matIndex].texture, a_uv) * materials[a_matIndex].albedo;

	FragColor = textureColor * vec4(lightResult * a_color, 1.0f);

}

vec3 LightColor(vec3 direction, vec3 color, vec3 normal, vec3 viewDir) {

	vec3 ambient = color * ambientStrength;

	vec3 diffuse = max(dot(normal, direction), 0.0f) * color;

	vec3 reflectDir = reflect(direction, normal);
	vec3 specular = pow(max(dot(viewDir, reflectDir), 0.0f), 32) * specularStrength * color;

	return ambient + diffuse + specular;

}
vec3 AmbientLightColor(vec3 direction, vec3 color, vec3 normal) {

	vec3 ambient = color * ambientStrength;
	vec3 diffuse = max(dot(normal, direction), 0.0f) * color * ambientStrength;

	return ambient + diffuse;

}