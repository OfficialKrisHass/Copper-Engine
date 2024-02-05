#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

uniform mat4 ProjectionView;

out vec3 a_color;

void main() {

    a_color = color;
    gl_Position = ProjectionView * vec4(position, 1.0f);

}