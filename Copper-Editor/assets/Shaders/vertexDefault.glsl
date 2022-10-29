#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;

uniform mat4 View;
uniform mat4 Projection;

out vec3 a_position;
out vec3 a_color;
out vec3 a_normal;

void main() {
	
	gl_Position = Projection * View * vec4(position, 1.0);
	
	a_position = position;
	a_color = color;
	a_normal = normal;

}