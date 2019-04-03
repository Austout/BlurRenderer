// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410

// interpolated colour received from vertex stage

uniform vec3 color;

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 BrightColor;

void main(void)
{
	BrightColor = vec4(color, 1.0);
	FragColor = vec4(color, 1.0);
}
