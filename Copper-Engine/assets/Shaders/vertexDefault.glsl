#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

uniform mat4 Model;

out vec3 a_color;

void main() {
	
	gl_Position = Model * vec4(position, 1.0);
	
	a_color = color;

}