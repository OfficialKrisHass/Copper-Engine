#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec3 a_color;

void main() {
	
	gl_Position = Projection * View * Model * vec4(position, 1.0);
	
	a_color = color;

}