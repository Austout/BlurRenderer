#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <glfw3.h>
#include <vector>
#include "texture.h"
struct Geometry
{
	// OpenGL names for array buffer objects, vertex array object
	GLuint  vertexBuffer;
	GLuint  textureBuffer;
	GLuint  normalBuffer;
	GLuint  colourBuffer;
	GLuint  vertexArray;
	GLsizei elementCount;
	MyTexture tex;
	// initialize object names to zero (OpenGL reserved value)
	Geometry() : normalBuffer(0), vertexBuffer(0), colourBuffer(0), vertexArray(0), elementCount(0)
	{}
};
class ModelOBJ {
public:
	Geometry model;
	glm::vec3 scale;
	glm::vec3 rotation;
	glm::vec3 translation;
	glm::vec3 colour;
	GLuint shader;
	ModelOBJ(glm::vec3 pos, glm::vec3 colour, glm::vec3 rotation, glm::vec3 scale, Geometry model, GLuint shader) {
		ModelOBJ::rotation = rotation;
		ModelOBJ::scale = (scale);
		ModelOBJ::translation = (pos);
		ModelOBJ::colour = colour;
		ModelOBJ::shader = shader;
		ModelOBJ::model = model;
	}
	ModelOBJ() {

	}
};

void Gen3dModel(const char * filename, const char *texName, Geometry &geometry);
void generateModel(float radius, float angle, Geometry &geometry);
bool CheckGLErrors();
void generatePlane(Geometry &geometry,float size,float subSteps);
void generatePlane(Geometry &geometry);
void render(Geometry model,GLuint shader,glm::mat4 view, glm::mat4 proj, glm::vec3 pos, glm::vec3 rot, glm::vec3 colourCurrent, glm::vec3 scale);
