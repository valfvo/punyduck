#version 330 core

in vec2 vTexPos;
in vec4 vColor;

out vec4 fColor;

uniform sampler2D texture0;

void main() {
	fColor = vColor * texture(texture0, vTexPos);
}