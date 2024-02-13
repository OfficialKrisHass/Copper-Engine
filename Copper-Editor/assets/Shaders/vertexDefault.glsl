#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 uv;
layout(location = 4) in float matIndex;

uniform mat4 ProjectionView;

out vec3 a_position;
out vec3 a_color;
out vec3 a_normal;
out vec2 a_uv;
flat out int a_matIndex;

void main() {
	
	gl_Position = ProjectionView * vec4(position, 1.0);
	
	a_position = position;
	a_color = color;
	a_normal = normal;
	a_uv = uv;
	a_matIndex = int(matIndex);

}