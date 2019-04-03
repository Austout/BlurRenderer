// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410

// interpolated colour received from vertex stage

uniform sampler2D mTexture;
uniform sampler2D bloomBlur;

// first output is mapped to the framebuffer's colour index by default
out vec4 FragmentColour;
in vec2 texCoord;

void main(void)
{
	const float gamma = 2.2;
	const float exposure = 1.0;

	vec3 color = texture(mTexture, texCoord).rgb;
	vec3 bloomColor = texture(bloomBlur, texCoord).rgb;
	float bloomThreshold = bloomColor.length() > 1 ? 1 : bloomColor.length();
	color = bloomColor + ((1 - bloomThreshold)*color) + color; // additive blending
	
	// reinhard tone mapping
	//vec3 mapped = color / (color + vec3(1.0));
	// gamma correction 
	// Exposure tone mapping
    //vec3 mapped = vec3(1.0) - exp(-color * exposure);
    // Gamma correction 
	vec3 mapped = pow(color, vec3(1.0 / gamma));

	FragmentColour = vec4(mapped, 1.0);
}
