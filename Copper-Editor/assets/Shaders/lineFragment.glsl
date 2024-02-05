#version 330 core

in vec3 a_color;

out vec4 fragColor;

void main() {

    fragColor = vec4(a_color, 1.0f);

}