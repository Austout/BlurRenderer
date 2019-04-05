#include "RenderManager.h"
#include "GeneralInclude.h"
#include "ModelOBJ.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <iterator>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace std;

RenderManager* RenderManager::pInstance = nullptr;
RenderManager* RenderManager::getInstance()
{
	if (nullptr == pInstance)
		pInstance = new RenderManager();

	return pInstance;
}

// --------------------------------------------------------------------------
// OpenGL shader support functions

// reads a text file with the given name into a string
string LoadSource(const string &filename)
{
	string source;

	ifstream input(filename.c_str());
	if (input) {
		copy(istreambuf_iterator<char>(input),
			istreambuf_iterator<char>(),
			back_inserter(source));
		input.close();
	}
	else {
		std::cout << "ERROR: Could not load shader source from file " << filename << endl;
	}

	return source;
}

// creates and returns a shader object compiled from the given source
GLuint CompileShader(GLenum shaderType, const string &source)
{
	// allocate shader object name
	GLuint shaderObject = glCreateShader(shaderType);

	// try compiling the source as a shader of the given type
	const GLchar *source_ptr = source.c_str();
	glShaderSource(shaderObject, 1, &source_ptr, 0);
	glCompileShader(shaderObject);

	// retrieve compile status
	GLint status;
	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint length;
		glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &length);
		string info(length, ' ');
		glGetShaderInfoLog(shaderObject, info.length(), &length, &info[0]);
		std::cout << "ERROR compiling shader:" << endl << endl;
		std::cout << source << endl;
		std::cout << info << endl;
	}

	return shaderObject;
}
// creates and returns a program object linked from vertex and fragment shaders
GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader)
{
	// allocate program object name
	GLuint programObject = glCreateProgram();

	// attach provided shader objects to this program
	if (vertexShader)   glAttachShader(programObject, vertexShader);
	if (fragmentShader) glAttachShader(programObject, fragmentShader);

	// try linking the program with given attachments
	glLinkProgram(programObject);

	// retrieve link status
	GLint status;
	glGetProgramiv(programObject, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint length;
		glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &length);
		string info(length, ' ');
		glGetProgramInfoLog(programObject, info.length(), &length, &info[0]);
		std::cout << "ERROR linking shader program:" << endl;
		std::cout << info << endl;
	}

	return programObject;
}
GLuint InitializeShaders(const char * vertexShaderName, const char * fragmentShaderName)
{
	// load shader source from files
	string vertexSource = LoadSource(vertexShaderName);
	string fragmentSource = LoadSource(fragmentShaderName);
	if (vertexSource.empty() || fragmentSource.empty()) return false;

	// compile shader source into shader objects
	GLuint vertex = CompileShader(GL_VERTEX_SHADER, vertexSource);
	GLuint fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

	// link shader program
	GLuint program = LinkProgram(vertex, fragment);

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	// check for OpenGL errors and return false if error occurred
	return program;
}

void RenderManager::renderColor(ModelOBJ model, glm::mat4 view, glm::mat4 proj) {

	glUseProgram(model.shader);
	glm::mat4 combined = proj * view;//glm::perspective((float)PI_F * 0.4f, float(1024) / float(1024), 0.0f, 5001.0f);

	glUniformMatrix4fv(glGetUniformLocation(model.shader, "modelViewProjection"), 1, false, glm::value_ptr(combined));

	glUniform3f(glGetUniformLocation(model.shader, "color"), model.colour.x, model.colour.y, model.colour.z);

	glUniform3f(glGetUniformLocation(model.shader, "offset"), model.translation.x, model.translation.y, model.translation.z);

	glm::mat4 scale;
	scale = glm::scale(scale, model.scale);
	glm::mat4 rotation = glm::toMat4(glm::quat(model.rotation));

	glUniformMatrix4fv(glGetUniformLocation(model.shader, "rotation"), 1, false, glm::value_ptr(rotation));
	glUniformMatrix4fv(glGetUniformLocation(model.shader, "scale"), 1, false, glm::value_ptr(scale));


	glBindVertexArray(model.model.vertexArray);

	glDrawArrays(GL_TRIANGLES, 0, model.model.elementCount);
	CheckGLErrors();
	// reset state to default (no shader or geometry bound)
	glBindVertexArray(0);
	glUseProgram(0);
}

void RenderManager::renderTexture(ModelOBJ model, glm::mat4 view, glm::mat4 proj,glm::vec3 camPos)
{
	glUseProgram(model.shader);
	glm::mat4 combined = proj * view;//glm::perspective((float)PI_F * 0.4f, float(1024) / float(1024), 0.0f, 5001.0f);

	glUniform1i(glGetUniformLocation(model.shader, "mTexture"), 0);

	glUniformMatrix4fv(glGetUniformLocation(model.shader, "modelViewProjection"), 1, false, glm::value_ptr(combined));

	glUniform3f(glGetUniformLocation(model.shader, "offset"), model.translation.x, model.translation.y, model.translation.z);

	glm::mat4 scale;
	scale = glm::scale(scale, model.scale);
	glm::mat4 rotation = glm::toMat4(glm::quat(model.rotation));

	glUniformMatrix4fv(glGetUniformLocation(model.shader, "rotation"), 1, false, glm::value_ptr(rotation));
	glUniformMatrix4fv(glGetUniformLocation(model.shader, "scale"), 1, false, glm::value_ptr(scale));
	
	Light light = lights.at(0);
	vector<float> intensity;
	vector<glm::vec3> pos;
	vector<glm::vec3> color;

	for (int i = 0; i < lights.size(); i++) {
		intensity.push_back(lights.at(i).intensity);
		pos.push_back(lights.at(i).model.translation);
		color.push_back(lights.at(i).model.colour);
	}
	intensity.push_back(-1.0f);
	glUniform3fv(glGetUniformLocation(model.shader, "lightPos"), pos.size(),(GLfloat*) &pos[0]);
	glUniform3fv(glGetUniformLocation(model.shader, "lightColor"), color.size(), (GLfloat*) &color[0]);
	glUniform3f(glGetUniformLocation(model.shader, "viewPos"), camPos.x, camPos.y, camPos.z);
	glUniform1fv(glGetUniformLocation(model.shader, "intensity"), intensity.size(), &intensity[0]);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, model.model.tex.textureID);

	glBindVertexArray(model.model.vertexArray);

	glDrawArrays(GL_TRIANGLES, 0, model.model.elementCount);
	CheckGLErrors();

	glBindTexture(GL_TEXTURE_2D, 0);
	// reset state to default (no shader or geometry bound)
	glBindVertexArray(0);
	glUseProgram(0);

	// check for an report any OpenGL errors
	//CheckGLErrors();
}
void RenderManager::renderFrameBuffer(ModelOBJ *model, GLuint bloomTex)
{
	glUseProgram(model->shader);

	glUniform1i(glGetUniformLocation(model->shader, "mTexture"), 0);
	glUniform1i(glGetUniformLocation(model->shader, "bloomBlur"), 1);


	glm::mat4 scale;
	scale = glm::scale(scale, model->scale);
	glm::mat4 rotation = glm::toMat4(glm::quat(model->rotation));

	glUniform3f(glGetUniformLocation(model->shader, "offset"), model->translation.x, model->translation.y, model->translation.z);
	glUniformMatrix4fv(glGetUniformLocation(model->shader, "rotation"), 1, false, glm::value_ptr(rotation));
	glUniformMatrix4fv(glGetUniformLocation(model->shader, "scale"), 1, false, glm::value_ptr(scale));


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, model->model.tex.textureID);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, bloomTex);

	glBindVertexArray(model->model.vertexArray);

	glDrawArrays(GL_TRIANGLES, 0, model->model.elementCount);
	CheckGLErrors();

	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	// reset state to default (no shader or geometry bound)
	glBindVertexArray(0);
	glUseProgram(0);

	// check for an report any OpenGL errors
	//CheckGLErrors();
}
void getGuassianBlur1D(vector<GLfloat> *mfloats, int n, float sigma)
{
	mfloats->clear();
	for (int x = 0; x < n; x++) {
		float newX = x + ceil((float)n / 2) - n;
		mfloats->push_back((float)pow(2.71828182845904523536, -((newX*newX)) / (2 * sigma*sigma))
			/ sqrt(2 * PI_F *sigma*sigma));
	}
}


void RenderManager::renderBlur(ModelOBJ *model, bool horizontal)
{
	glUseProgram(model->shader);

	glUniform1i(glGetUniformLocation(model->shader, "mTexture"), 0);


	glm::mat4 scale;
	scale = glm::scale(scale, model->scale);
	glm::mat4 rotation = glm::toMat4(glm::quat(model->rotation));
	vector<GLfloat> mFloats;
	getGuassianBlur1D(&mFloats, KERNEL_SIZE_BLUR, KERNEL_SIGMA);

	glUniform1i(glGetUniformLocation(model->shader, "matrixSize"), KERNEL_SIZE_BLUR);
	glUniform1fv(glGetUniformLocation(model->shader, "blurValues"), mFloats.size(), &mFloats[0]);

	glUniform1i(glGetUniformLocation(model->shader, "horizontal"), horizontal);
	glUniform3f(glGetUniformLocation(model->shader, "offset"), model->translation.x, model->translation.y, model->translation.z);
	glUniformMatrix4fv(glGetUniformLocation(model->shader, "rotation"), 1, false, glm::value_ptr(rotation));
	glUniformMatrix4fv(glGetUniformLocation(model->shader, "scale"), 1, false, glm::value_ptr(scale));


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, model->model.tex.textureID);

	glBindVertexArray(model->model.vertexArray);

	glDrawArrays(GL_TRIANGLES, 0, model->model.elementCount);
	CheckGLErrors();

	glBindTexture(GL_TEXTURE_2D, 0);
	// reset state to default (no shader or geometry bound)
	glBindVertexArray(0);
	glUseProgram(0);

	// check for an report any OpenGL errors
	//CheckGLErrors();
}
void RenderManager::generateFrameBuffer(GLuint *frameBuffer, GLuint *frameBufferTexture,GLuint *rboDepth,int width,int height) {
	glGenFramebuffers(1, frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, *frameBuffer);
	glGenTextures(2, frameBufferTexture);
	glGenRenderbuffers(1, rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, *rboDepth);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	// attach buffers
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, frameBufferTexture[i]);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// attach texture to framebuffer
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, frameBufferTexture[i], 0
		);
	}

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, *rboDepth);

	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void RenderManager::generateFrameBuffer(GLuint *frameBuffer, GLuint *frameBufferTexture, int width, int height) {
	glGenFramebuffers(1, frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, *frameBuffer);
	glGenTextures(1, frameBufferTexture);
	glBindTexture(GL_TEXTURE_2D, frameBufferTexture[0]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
		frameBufferTexture[0], 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

RenderManager::~RenderManager() {
	/*glDeleteTextures(1, hdrTexture->frameBufferTexture);
	glDeleteFramebuffers(1, hdrTexture->frameBuffer);
	delete hdrTexture;*/
}
void RenderManager::initScene() {
	// call function to load and compile shader programs
	program = InitializeShaders("../shaders/vertex.glsl", "../shaders/fragment.glsl");
	programColor = InitializeShaders("../shaders/colorVertex.glsl", "../shaders/colorFragment.glsl");
	programFramebuffer = InitializeShaders("../shaders/framebufferVertex.glsl", "../shaders/framebufferFragment.glsl");
	programblur = InitializeShaders("../shaders/framebufferVertex.glsl", "../shaders/blurFragment.glsl");
	if (program == 0 || programFramebuffer == 0 || programblur == 0) {
		std::cout << "Program could not initialize shaders, TERMINATING" << std::endl;
		return;
	}
	generateFrameBuffer(&hdrTexture.frameBuffer, &(hdrTexture.frameBufferTexture[0]), &hdrTexture.depthBuffer,WINDOW_WIDTH,WINDOW_HEIGHT);
	generateFrameBuffer(&pingPong[0].frameBuffer, &(pingPong[0].frameBufferTexture[0]), WINDOW_WIDTH, WINDOW_HEIGHT);
	generateFrameBuffer(&pingPong[1].frameBuffer, &(pingPong[1].frameBufferTexture[0]), WINDOW_WIDTH, WINDOW_HEIGHT);
	Geometry box;
	Gen3dModel("../res/box.obj", "../res/floor.png", box);

	Geometry hoverCraft;
	Gen3dModel("../res/HoverCar.obj", "../res/earth.png", hoverCraft);

	Geometry plane;
	generatePlane(plane,1,0.1f);
	plane.tex = box.tex;
	ModelOBJ boxOBJ(glm::vec3(0, -3, 0), glm::vec3(1, 1, 1), glm::vec3(PI_F / 2.0f, 0, 0), glm::vec3(100, 100, 1), plane, program);
	texModels.push_back(boxOBJ);

	ModelOBJ hoverCraftOBJ(glm::vec3(15, -1, 0), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), glm::vec3(0.01f, 0.01f, 0.01f), hoverCraft, program);
	texModels.push_back(hoverCraftOBJ);

	Light boxColor(1, glm::vec3(1, 1, 1),glm::vec3(0, 0, 0), glm::vec3(10,10,10), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), box, programColor);
	lights.push_back(boxColor);

	Light boxColor2(10, glm::vec3(10, 0.75, 0.75),glm::vec3(-10, 0, 0), glm::vec3(10, 0.75, 0.75), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), box, programColor);
	lights.push_back(boxColor2);
	Geometry plane2;
	generatePlane(plane2);
	plane2.tex.textureID = hdrTexture.frameBufferTexture[0];
	hdrPlane = new ModelOBJ(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), glm::vec3(4, 4, 1), plane2, programFramebuffer);
}
void RenderManager::renderScene(glm::mat4 viewMatrix, glm::mat4 perspectiveMatrix,glm::vec3 camPos) {
	glBindFramebuffer(GL_FRAMEBUFFER, hdrTexture.frameBuffer);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		for (ModelOBJ model : texModels) {
			renderTexture(model, viewMatrix, perspectiveMatrix, camPos);
		}
		for (Light light : lights) {
			renderColor(light.model, viewMatrix, perspectiveMatrix);
		}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	hdrPlane->model.tex.textureID = hdrTexture.frameBufferTexture[1];
	hdrPlane->shader = programblur;
	for (int i = 0; i < 10;i++) {
		glBindFramebuffer(GL_FRAMEBUFFER, pingPong[i%2].frameBuffer);
		renderBlur(hdrPlane, i%2);
		hdrPlane->model.tex.textureID = *(pingPong[i%2].frameBufferTexture);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0, 0, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	hdrPlane->shader = programFramebuffer;
	hdrPlane->model.tex.textureID = hdrTexture.frameBufferTexture[0];
	renderFrameBuffer(hdrPlane, *(pingPong[0].frameBufferTexture));

}