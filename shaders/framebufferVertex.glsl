// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410

// location indices for these attributes correspond to those specified in the
// InitializeGeometry() function of the main program
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec2 VertexTexCoord;

uniform vec3 offset;
uniform mat4 rotation;
uniform mat4 scale;
out vec2 texCoord;

void main()
{
	vec4 position = (vec4(offset, 1) + rotation * scale*vec4(VertexPosition, 1));
	gl_Position = position;
	texCoord = VertexTexCoord;
}