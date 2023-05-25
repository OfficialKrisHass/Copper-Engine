#version 330 core

in vec2 coords;

uniform sampler2D tex;

out vec4 col;

void main() {
	
	//col = texture(tex, coords) * vec4(1.0f);
	col = vec4(1.0f, 0.0f, 0.0f, 1.0f);

}