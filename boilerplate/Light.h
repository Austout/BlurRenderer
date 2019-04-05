#pragma once
#include "ModelOBJ.h"
class Light {
public:
	Light(float intensity,glm::vec3 lightColor,glm::vec3 pos, glm::vec3 colourObj, glm::vec3 rotation, glm::vec3 scale, Geometry model, GLuint shader) {
		Light::model = ModelOBJ(pos, colourObj, rotation,scale, model, shader);
		Light::intensity = intensity;
		Light::lightColor = lightColor;
	}
	glm::vec3 lightColor;
	float intensity;
	ModelOBJ model;
};