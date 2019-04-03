// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410

// interpolated colour received from vertex stage
in vec3 Colour;
// first output is mapped to the framebuffer's colour index by default
out vec4 FragmentColour;

uniform vec2 val;

void main(void)
{
	//Finalposition.x = inPositionx * cos ( 2pi*inPositionX /500)
    // write colour output without modification
    FragmentColour = vec4(Colour.x + val.x,Colour.y + val.y,Colour.z, 0);
}
