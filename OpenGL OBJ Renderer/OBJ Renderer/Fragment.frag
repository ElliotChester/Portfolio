// Rename this file "Wk10-1 Fragment.frag"
//fragment shader
#version 430 core

// IN variable passed from vertex to fragment, this variable is defined in vertex shader.
in vec3 Color;

out vec4 FragColor;

void main() {
	FragColor = vec4(Color, 1.0);
}