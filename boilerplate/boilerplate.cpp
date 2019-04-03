// ==========================================================================
// Barebones OpenGL Core Profile Boilerplate
//    using the GLFW windowing system (http://www.glfw.org)
//
// Loosely based on
//  - Chris Wellons' example (https://github.com/skeeto/opengl-demo) and
//  - Camilla Berglund's example (http://www.glfw.org/docs/latest/quick.html)
//
// Author:  Sonny Chan, University of Calgary
// Co-Authors:
//			Jeremy Hart, University of Calgary
//			John Hall, University of Calgary
// Date:    December 2015
// ==========================================================================

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <iterator>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ModelOBJ.h"
#include "texture.h"
#include "Camera.h"
#include "GeneralInclude.h"
#include "RenderManager.h"

using namespace std;
using namespace glm;
// --------------------------------------------------------------------------
// OpenGL utility and support function prototypes


void QueryGLVersion();

Camera camera = Camera(WINDOW_WIDTH, WINDOW_HEIGHT);

string LoadSource(const string &filename);
GLuint CompileShader(GLenum shaderType, const string &source);
GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader);

bool lbPushed = false;
// --------------------------------------------------------------------------
// Functions to set up OpenGL shader programs for rendering

// load, compile, and link shaders, returning true if successful

// --------------------------------------------------------------------------
// GLFW callback functions

// reports GLFW errors
void ErrorCallback(int error, const char* description)
{
	std::cout << "GLFW ERROR " << error << ":" << std::endl;
	std::cout << description << endl;
}

// handles keyboard input events
glm::vec3 offset(0,0,0);
#define MOVEMENT_RATE 0.5f;
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	
}

float getTriangleNum(float angle){
	if(180.0f / angle != round(180.0f / angle)){
		std::cout << "invalid angle" << std::endl;
	}
	return (360/angle) * (180/angle) * 2;
}

void handleInput(float delta, GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
		vec3 pos = camera.getLookPos();
		pos.x += MOVEMENT_RATE;
		camera.setLookPos(pos);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
		vec3 pos = camera.getLookPos();
		pos.x -= MOVEMENT_RATE;
		camera.setLookPos(pos);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		vec3 pos = camera.getLookPos();
		pos.z += MOVEMENT_RATE;
		camera.setLookPos(pos);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		vec3 pos = camera.getLookPos();
		std::cout << pos.z << std::endl;
		pos.z -= MOVEMENT_RATE;
		camera.setLookPos(pos);

	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		vec3 pos = camera.getLookPos();
		pos.y += MOVEMENT_RATE;
		camera.setLookPos(pos);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		vec3 pos = camera.getLookPos();
		pos.y -= MOVEMENT_RATE;
		camera.setLookPos(pos);
	}


}
int main(int argc, char *argv[])
{
	// initialize the GLFW windowing system
	if (!glfwInit()) {
		std::cout << "ERROR: GLFW failed to initialize, TERMINATING" << endl;
		return -1;
	}
	glfwSetErrorCallback(ErrorCallback);

	// attempt to create a window with an OpenGL 4.1 core profile context
	GLFWwindow *window = 0;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "CPSC 585 project", 0, 0);
	if (!window) {
		std::cout << "Program failed to create GLFW window, TERMINATING" << endl;
		glfwTerminate();
		return -1;
	}

	// set keyboard callback function and make our context current (active)
	glfwSetKeyCallback(window, KeyCallback);
	glfwMakeContextCurrent(window);

	//Intialize GLAD
	if (!gladLoadGL())
	{
		std::cout << "GLAD init failed" << endl;
		return -1;
	}

	// query and print out information about our OpenGL environment
	QueryGLVersion();




	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// three vertex positions and assocated colours of a triangle
	float angle = 3;
	float radius = 1;


	// Geometry geometry;
	// sun to earth : 149.6 * 1 000 000 million km
	// earth to moon: 384,400 km
	// log_10(2x)
	// sun to earth : 3.47596
	// earth to moon: 0.885813


	// sun = 109xearth (approximatly)
	// multiply size by 10 of all and use base 2000 =
	// sun = 0.920145
	// earth = 0.302936f
	// moon = 0.1306755f

	// rotation
	// sun : 2.447 seconds
	// earth : 0.1 seconds
	// moon: 2.7322 seconds

	// earth: 36.5s
	// moon: 2.7
	// sun: N/A
	RenderManager::getInstance()->initScene();
	double lastTime = 0;
	//glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	// TODO limit max x and y
	// run an event-triggered main loop
	mat4 perspectiveMatrix = camera.getPerspectiveMat();
	int frameRate = 0;
	float sumSecond = 0;
	while (!glfwWindowShouldClose(window))
	{
		////////////////////////
		//Camera interaction
		////////////////////////
		//Translation
		double delta = glfwGetTime() - lastTime;
		lastTime = glfwGetTime();
		sumSecond += delta;
		if (sumSecond > 1) {
			sumSecond = 0;
			std::cout << "frameRate" << frameRate <<"\n";
			frameRate = 0;
		}
		frameRate++;
		///////////
		//Drawing
		//////////
		// clear screen to a dark grey colour
		//std::cout << "delta" << delta <<"\n";
		handleInput(delta,window);
		mat4 viewMatrix = camera.getToCameraMat();
		RenderManager::getInstance()->renderScene(viewMatrix, perspectiveMatrix,camera.getLookPos());

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	// clean up allocated resources before exit
	glUseProgram(0);
	//glDeleteProgram(program);
	glfwDestroyWindow(window);
	glfwTerminate();
	char c;
	std::cout << "press q then \\n key to close" << endl;
	std::cin.get(c);
	return 0;
}

// ==========================================================================
// SUPPORT FUNCTION DEFINITIONS

// --------------------------------------------------------------------------
// OpenGL utility functions

void QueryGLVersion()
{
	// query opengl version and renderer information
	string version = reinterpret_cast<const char *>(glGetString(GL_VERSION));
	string glslver = reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));
	string renderer = reinterpret_cast<const char *>(glGetString(GL_RENDERER));

	std::cout << "OpenGL [ " << version << " ] "
		<< "with GLSL [ " << glslver << " ] "
		<< "on renderer [ " << renderer << " ]" << endl;
}
