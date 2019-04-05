// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410

// interpolated colour received from vertex stage
//uniform vec3 Colour;
//uniform sampler2D mTexture;

// first output is mapped to the framebuffer's colour index by default
out vec4 FragmentColour;
//in vec2 texCoord;
//in float isShaded;
//in float normal;
void main(void)
{
    // write colour output without modification
    //FragmentColour = vec4(Colour, 0);
    //vec4 color = vec4(1,1,1,1);//texture(mTexture, texCoord);
    //color.x /= isShaded;
    //color.y /= isShaded; 
    //color.z /= isShaded;
    


    FragmentColour = vec4(1,1,1,1);// * max(0,normal);
}
