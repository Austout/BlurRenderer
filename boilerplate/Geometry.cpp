#include "Geometry.h"

glm::vec2 Line::getPerpendicular(){
	glm::vec2 line = b-a;
	return normalize(glm::vec2(-line.y, line.x));
}

Line::Line():a(0), b(0)
{
}

Line::Line(glm::vec2 a, glm::vec2 b):a(a), b(b){}

float Line::length()
{
	return sqrt(dot(a-b, a-b));
}