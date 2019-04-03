#pragma once
#include "ModelOBJ.h"
class Light {
public:
	Light(int matrixSize, float sigma, glm::vec3 projColor, glm::vec3 pos, glm::vec3 colour, glm::vec3 rotation, glm::vec3 scale, Geometry model, GLuint shader) {
		Light::model = ModelOBJ(pos, colour, rotation,scale, model, shader);
		Light::matrixSize = matrixSize;
		Light::sigma = sigma;
		Light::projColor = projColor;

	}
	int matrixSize;
	float sigma;
	glm::vec3 projColor;
	ModelOBJ model;
};