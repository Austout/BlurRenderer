// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410

// interpolated colour received from vertex stage

uniform sampler2D mTexture;

// first output is mapped to the framebuffer's colour index by default
out vec4 FragmentColour;
in vec2 texCoord;

uniform bool horizontal;
uniform float blurValues[1000];
uniform int matrixSize;
void main(void)
{
	const float weightMul = 1.0f;
	vec2 tex_offset = 1.0 / textureSize(mTexture, 0); // gets size of single texel
	vec3 result = texture(mTexture, texCoord).rgb * blurValues[0]; // current fragment's contribution
	if (horizontal)
	{
		for (int i = 1; i < matrixSize; ++i)
		{
			result += texture(mTexture, texCoord + vec2(tex_offset.x * i, 0.0)).rgb * blurValues[i] * weightMul;
			result += texture(mTexture, texCoord - vec2(tex_offset.x * i, 0.0)).rgb * blurValues[i] * weightMul;
		}
	}
	else
	{
		for (int i = 1; i < matrixSize; ++i)
		{
			result += texture(mTexture, texCoord + vec2(0.0, tex_offset.y * i)).rgb * blurValues[i] * weightMul;
			result += texture(mTexture, texCoord - vec2(0.0, tex_offset.y * i)).rgb * blurValues[i] * weightMul;
		}
	}
	FragmentColour = vec4(result, 1.0);
}
