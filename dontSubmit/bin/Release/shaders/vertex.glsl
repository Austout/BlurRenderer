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
//layout(location = 1) in vec3 VertexTexCoord;

uniform mat4 modelViewProjection;
//uniform float objects[12]; //global
// first three are the position of the light
// next three are the position of the earth
// 6 is the size of the earth
// next three are the position of the moon
// 10 is the size of the moon
//out vec2 texCoord;
//out float isShaded;
//out float normal;
//float getIntersectionSphere(vec3 ray, vec3 offset,vec3 points,float size){
//	vec3 c = points;
//	float b = dot(ray, offset -c);
//	float raySquare = (length(ray) * length(ray));
//	float value = (b*b) - raySquare*(length(offset -c)*length(offset -c)) + raySquare*(size*size);
//	if(value > 0){
//		float t = (-b - sqrt(value)) / raySquare;
//		return t;
//	}
//	return -1;
//}

void main()
{
    // assign vertex position without modification
    //vec3 lightPos = vec3(objects[0],objects[1],objects[2]);
    //vec3 earthPos = vec3(objects[3],objects[4],objects[5]);
    vec4 position = modelViewProjection*vec4(VertexPosition, 1.0);
    gl_Position = position;
    //vec3 localPosition = vec3(position.x,position.y,position.z);
    //normal = dot(normalize(localPosition-earthPos),normalize(lightPos - vec3(position.x,position.y,position.z)));
    //isShaded = 1;
    //if(objects[11] < 0.5f){
     //   normal = 1;
    //}
    //texCoord = VertexTexCoord;
}