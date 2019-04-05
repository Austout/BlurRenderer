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
	vec3 mappedBloom = vec3(1.0) - exp(-bloomColor * exposure);
	mappedBloom = pow(mappedBloom, vec3(1.0 / gamma));
	color = mappedBloom + ((1 - mappedBloom)*color); // additive blending
	
	// reinhard tone mapping
	//vec3 mapped = color / (color + vec3(1.0));
	// gamma correction 
	// Exposure tone mapping
    // Gamma correction 

	FragmentColour = vec4(color, 1.0);
}
