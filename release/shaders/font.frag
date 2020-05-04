#version 330 core

in vec2 vTexPos;
in vec4 vColor;

out vec4 fColor;

uniform sampler2D texture0;

void main() {	
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(texture0, vTexPos).r);
    fColor = vColor * sampled;
}
