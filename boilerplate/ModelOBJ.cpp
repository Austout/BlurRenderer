#include "ModelOBJ.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "GeneralInclude.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

// deallocate geometry-related objects
void DestroyGeometry(Geometry *geometry)
{
	// unbind and destroy our vertex array object and associated buffers
	//glBindVertexArray(0);
	//glDeleteVertexArrays(1, &geometry->vertexArray);
	//glDeleteBuffers(1, &geometry->vertexBuffer);
}
bool CheckGLErrors()
{
	bool error = false;
	for (GLenum flag = glGetError(); flag != GL_NO_ERROR; flag = glGetError())
	{
		std::cout << "OpenGL ERROR:  ";
		switch (flag) {
		case GL_INVALID_ENUM:
			std::cout << "GL_INVALID_ENUM" << std::endl; break;
		case GL_INVALID_VALUE:
			std::cout << "GL_INVALID_VALUE" << std::endl; break;
		case GL_INVALID_OPERATION:
			std::cout << "GL_INVALID_OPERATION" << std::endl; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl; break;
		case GL_OUT_OF_MEMORY:
			std::cout << "GL_OUT_OF_MEMORY" << std::endl; break;
		default:
			std::cout << "[unknown error code]" << std::endl;
		}
		error = true;
	}
	return error;
}

bool InitializeVAO(Geometry *geometry) {

	const GLuint VERTEX_INDEX = 0;
	const GLuint NORMAL_INDEX = 1;
	const GLuint TEX_COORD_INDEX = 2;

	//Generate Vertex Buffer Objects
	// create an array buffer object for storing our vertices
	glGenBuffers(1, &geometry->vertexBuffer);
	glGenBuffers(1, &geometry->normalBuffer);
	glGenBuffers(1, &geometry->textureBuffer);

	//Set up Vertex Array Object
	// create a vertex array object encapsulating all our vertex attributes
	glGenVertexArrays(1, &geometry->vertexArray);
	glBindVertexArray(geometry->vertexArray);

	// associate the position array with the vertex array object
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBuffer);
	glVertexAttribPointer(
		VERTEX_INDEX,		//Attribute index 
		3, 					//# of components
		GL_FLOAT, 			//Type of component
		GL_FALSE, 			//Should be normalized?
		sizeof(float) * 3,		//Stride - can use 0 if tightly packed
		0);					//Offset to first element
	glEnableVertexAttribArray(VERTEX_INDEX);

	glBindBuffer(GL_ARRAY_BUFFER, geometry->normalBuffer);
	glVertexAttribPointer(
		NORMAL_INDEX,		//Attribute index 
		3, 					//# of components
		GL_FLOAT, 			//Type of component
		GL_FALSE, 			//Should be normalized?
		sizeof(float) * 3, 		//Stride - can use 0 if tightly packed
		0);					//Offset to first element
	glEnableVertexAttribArray(NORMAL_INDEX);


	glBindBuffer(GL_ARRAY_BUFFER, geometry->textureBuffer);
	glVertexAttribPointer(
		TEX_COORD_INDEX,		//Attribute index 
		2, 					//# of components
		GL_FLOAT, 			//Type of component
		GL_FALSE, 			//Should be normalized?
		sizeof(float) * 2, 		//Stride - can use 0 if tightly packed
		0);					//Offset to first element
	glEnableVertexAttribArray(TEX_COORD_INDEX);
	// unbind our buffers, resetting to default state
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return !CheckGLErrors();
}
bool LoadGeometry(Geometry *geometry, float *vertices, int elementCount)
{
	geometry->elementCount = elementCount;

	// create an array buffer object for storing our vertices
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * geometry->elementCount, vertices, GL_STATIC_DRAW);

	//Unbind buffer to reset to default state
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// check for OpenGL errors and return false if error occurred
	return !CheckGLErrors();
}
bool LoadGeometry(Geometry *geometry, glm::vec3 *vertices, int elementCount)
{
	geometry->elementCount = elementCount;

	// create an array buffer object for storing our vertices
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * geometry->elementCount, vertices, GL_STATIC_DRAW);

	//Unbind buffer to reset to default state
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// check for OpenGL errors and return false if error occurred
	return !CheckGLErrors();
}
bool LoadGeometry(Geometry *geometry, float *vertices, float *normals, int elementCount)
{
	geometry->elementCount = elementCount;

	// create an array buffer object for storing our vertices
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * geometry->elementCount, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, geometry->normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * geometry->elementCount, normals, GL_STATIC_DRAW);

	//Unbind buffer to reset to default state
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// check for OpenGL errors and return false if error occurred
	return !CheckGLErrors();
}
bool LoadGeometry(Geometry *geometry, float *vertices, float *normals,float *texCoords, int elementCount)
{
	geometry->elementCount = elementCount;

	// create an array buffer object for storing our vertices
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * geometry->elementCount, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, geometry->normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * geometry->elementCount, normals, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, geometry->textureBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * geometry->elementCount, texCoords, GL_STATIC_DRAW);
	//Unbind buffer to reset to default state
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// check for OpenGL errors and return false if error occurred
	return !CheckGLErrors();
}
void Gen3dModel(const char * filename,const char *texName, Geometry &model) {
	std::cout << "add3dModel Started: " << filename << std::endl;
	std::ifstream indata;
	indata.open(filename);
	bool hasNormals = false;
	bool hasTextCoords = false;
	std::vector< float > uvIndices, normalIndices;
	std::vector< float > vertexIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;
	while (indata) {

		char lineHeader;
		char temp;
		temp = indata.get();
		while (temp != ' ' && temp != EOF && temp != '#') {
			lineHeader = temp;
			temp = indata.get();
		}
		if (temp == EOF) {
			break;
		}
		if (temp == '#') {
			while (temp != '\n') {
				temp = indata.get();
			}
			//do nothing it's a comment (just like this :)
		}
		else if (lineHeader == 'v') {
			glm::vec3 vertex;
			indata >> vertex.x;
			indata >> vertex.y;
			indata >> vertex.z;
			temp_vertices.push_back(vertex);
		}
		else if (lineHeader == 't') {
			glm::vec2 uv;
			indata >> uv.x;
			indata >> uv.y;
			temp_uvs.push_back(uv);
		}
		else if (lineHeader == 'n') {
			glm::vec3 normal;
			indata >> normal.x;
			indata >> normal.y;
			indata >> normal.z;
			temp_normals.push_back(normal);
		}
		else if (lineHeader == 'f') {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			char junk;
			for (int i = 0; i < 3; i++) {
				indata >> vertexIndex[i];
				if (indata.peek() == '/') {
					indata >> junk;
					if (indata.peek() != '/') {
						indata >> uvIndex[i];
						hasTextCoords = true;
					}
					if (indata.peek() == '/') {
						indata >> junk;
						indata >> normalIndex[i];
						hasNormals = true;
					}
				}

			}
			//TODO go back and load using faces indexing
			vertexIndices.push_back(temp_vertices.at(vertexIndex[0] - 1).x);
			vertexIndices.push_back(temp_vertices.at(vertexIndex[0] - 1).y);
			vertexIndices.push_back(temp_vertices.at(vertexIndex[0] - 1).z);
			vertexIndices.push_back(temp_vertices.at(vertexIndex[1] - 1).x);
			vertexIndices.push_back(temp_vertices.at(vertexIndex[1] - 1).y);
			vertexIndices.push_back(temp_vertices.at(vertexIndex[1] - 1).z);
			vertexIndices.push_back(temp_vertices.at(vertexIndex[2] - 1).x);
			vertexIndices.push_back(temp_vertices.at(vertexIndex[2] - 1).y);
			vertexIndices.push_back(temp_vertices.at(vertexIndex[2] - 1).z);

			uvIndices.push_back(temp_uvs.at(uvIndex[0] - 1).x);
			uvIndices.push_back(temp_uvs.at(uvIndex[0] - 1).y);
			uvIndices.push_back(temp_uvs.at(uvIndex[1] - 1).x);
			uvIndices.push_back(temp_uvs.at(uvIndex[1] - 1).y);
			uvIndices.push_back(temp_uvs.at(uvIndex[2] - 1).x);
			uvIndices.push_back(temp_uvs.at(uvIndex[2] - 1).y);

			normalIndices.push_back(temp_normals.at(normalIndex[0] - 1).x);
			normalIndices.push_back(temp_normals.at(normalIndex[0] - 1).y);
			normalIndices.push_back(temp_normals.at(normalIndex[0] - 1).z);
			normalIndices.push_back(temp_normals.at(normalIndex[1] - 1).x);
			normalIndices.push_back(temp_normals.at(normalIndex[1] - 1).y);
			normalIndices.push_back(temp_normals.at(normalIndex[1] - 1).z);
			normalIndices.push_back(temp_normals.at(normalIndex[2] - 1).x);
			normalIndices.push_back(temp_normals.at(normalIndex[2] - 1).y);
			normalIndices.push_back(temp_normals.at(normalIndex[2] - 1).z);
		}
	}
	InitializeVAO(&model);
	if (!hasNormals) {
		LoadGeometry(&model, &vertexIndices[0], vertexIndices.size() / 3);
	}
	else if(!hasTextCoords){
		std::cout << "finished model load normals " << normalIndices.size() << " " << vertexIndices.size() << "\n";
		LoadGeometry(&model, &vertexIndices[0], &normalIndices[0], vertexIndices.size() / 3);
	}
	else {
		LoadGeometry(&model, &vertexIndices[0], &normalIndices[0],&uvIndices[0], vertexIndices.size() / 3);
		bool result = InitializeTexture(&(model.tex), texName, GL_TEXTURE_2D);
		std::cout << "result of texture load: " << (result == true? "true":"false") << std::endl;
	}
	std::cout << "finished model load" << "\n";
}
glm::vec3 getSphereVectorPoint(float sphereAzmith, float sphereTheta, float radius) {
	glm::vec4 temp = glm::rotate(glm::mat4(1.0f), sphereTheta, glm::vec3(0, 1, 0))* (glm::rotate(glm::mat4(1.0f), sphereAzmith, glm::vec3(1, 0, 0)) * glm::vec4(0, 0, radius, 0));
	return glm::vec3(temp.x, temp.y, temp.z);
}
void generateModel(float radius, float angle, Geometry &geometry) {
	std::vector<glm::vec3> points;
	std::vector<glm::vec2> textCoords;
	for (float sphereAzmith = -90; sphereAzmith < 90; sphereAzmith += angle) {
		for (float sphereTheta = 0; sphereTheta < 360; sphereTheta += angle) {
			points.push_back(getSphereVectorPoint((sphereAzmith / 360.0f) *(PI_F * 2), (sphereTheta / 360.0f) *(PI_F * 2), radius));
			points.push_back(getSphereVectorPoint((sphereAzmith / 360.0f) *(PI_F * 2), ((sphereTheta + angle) / 360.0f) *(PI_F * 2), radius));
			points.push_back(getSphereVectorPoint(((sphereAzmith + angle) / 360.0f) *(PI_F * 2), ((sphereTheta + angle) / 360.0f) *(PI_F * 2), radius));

			textCoords.push_back(glm::vec2((sphereTheta / 360.0f), ((sphereAzmith - 90) / -180.0f)));
			textCoords.push_back(glm::vec2(((sphereTheta + angle) / 360.0f), ((sphereAzmith - 90) / -180.0f)));
			textCoords.push_back(glm::vec2(((sphereTheta + angle) / 360.0f), (((sphereAzmith - 90) + angle) / -180.0f)));

			// triangle 2
			points.push_back(getSphereVectorPoint(((sphereAzmith + angle) / 360.0f) *(PI_F * 2), ((sphereTheta + angle) / 360.0f) *(PI_F * 2), radius));
			points.push_back(getSphereVectorPoint(((sphereAzmith + angle) / 360.0f) *(PI_F * 2), (sphereTheta / 360.0f) *(PI_F * 2), radius));
			points.push_back(getSphereVectorPoint((sphereAzmith / 360.0f) *(PI_F * 2), (sphereTheta / 360.0f) *(PI_F * 2), radius));

			textCoords.push_back(glm::vec2(((sphereTheta + angle) / 360.0f), (((sphereAzmith - 90) + angle) / -180.0f)));
			textCoords.push_back(glm::vec2((sphereTheta / 360.0f), (((sphereAzmith - 90) + angle) / -180.0f)));
			textCoords.push_back(glm::vec2((sphereTheta / 360.0f), ((sphereAzmith - 90) / -180.0f)));
		}
	}
	glm::vec3* sphereArray = &points[0];
	glm::vec2* textureCoordsArray = &textCoords[0];
	// call function to create and fill buffers with geometry data
	if (!InitializeVAO(&geometry))
		std::cout << "Program failed to intialize geometry!" << std::endl;

	if (!LoadGeometry(&geometry, sphereArray, points.size() / 3))
		std::cout << "Failed to load geometry" << std::endl;

	//InitializeTexture(&(model->tex),filename,GL_TEXTURE_2D);
	//model->geo = geometry;
}
void generatePlane(Geometry &geometry) {
	std::vector <float > points;
	std::vector<float > normals;
	std::vector<float > textCoords;

	points.push_back(-0.5f);
	points.push_back(0.5f);
	points.push_back(0);

	points.push_back(0.5f);
	points.push_back(0.5f);
	points.push_back(0);

	points.push_back(0.5f);
	points.push_back(-0.5f);
	points.push_back(0);

	points.push_back(-0.5f);
	points.push_back(0.5f);
	points.push_back(0);

	points.push_back(0.5f);
	points.push_back(-0.5f);
	points.push_back(0);

	points.push_back(-0.5f);
	points.push_back(-0.5f);
	points.push_back(0);

	textCoords.push_back(0);
	textCoords.push_back(1);

	textCoords.push_back(1);
	textCoords.push_back(1);

	textCoords.push_back(1);
	textCoords.push_back(0);

	textCoords.push_back(0);
	textCoords.push_back(1);

	textCoords.push_back(1);
	textCoords.push_back(0);

	textCoords.push_back(0);
	textCoords.push_back(0);

	normals.push_back(0);
	normals.push_back(0);
	normals.push_back(-1);

	normals.push_back(0);
	normals.push_back(0);
	normals.push_back(-1);

	normals.push_back(0);
	normals.push_back(0);
	normals.push_back(-1);

	normals.push_back(0);
	normals.push_back(0);
	normals.push_back(-1);

	normals.push_back(0);
	normals.push_back(0);
	normals.push_back(-1);

	normals.push_back(0);
	normals.push_back(0);
	normals.push_back(-1);


	float* sphereArray = &points[0];
	float* textureCoordsArray = &textCoords[0];
	// call function to create and fill buffers with geometry data
	if (!InitializeVAO(&geometry))
		std::cout << "Program failed to intialize geometry!" << std::endl;

	if (!LoadGeometry(&geometry, sphereArray,&normals[0], textureCoordsArray, points.size() / 3))
		std::cout << "Failed to load geometry" << std::endl;
}