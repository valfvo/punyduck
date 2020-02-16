#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texPos;
layout (location = 2) in vec4 color;

out vec2 vTexPos;
out vec4 vColor;

uniform mat4 projection;
uniform mat4 model;

void main() {
	gl_Position = projection * model * vec4(pos, 1.0);
	vTexPos = texPos;
	vColor = color;
}
