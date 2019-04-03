#pragma once

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <iterator>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include <vector>
#include "Geometry.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void RenderSceneSnowflake(Geometry *snowflake, GLuint program);

void RenderSceneFern(Geometry *fern, GLuint program);

void RenderSceneSquares(Geometry *square,Geometry *diamond, GLuint program, int currentLevel);

void renderTriangle(Geometry *triangle,GLuint program,float currentScale,float xPos, float yPos, int currentLevel);

void RenderSceneTriangles(Geometry *triangle, GLuint program,int currentLevel);

void RenderSceneSpiral(Geometry *spiral, GLuint program);

bool CheckGLErrors();
