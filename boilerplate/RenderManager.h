#pragma once
#include <vector>
#include "ModelOBJ.h"
#include "Light.h"
#define KERNEL_SIZE_BLUR 5
#define KERNEL_SIGMA KERNEL_SIZE_BLUR/1.5f
using namespace std;
struct FramebufferObject {
	GLuint frameBufferTexture[2];
	GLuint frameBuffer;
	GLuint depthBuffer;
};
class RenderManager final
{
public:
	// creates/returns singleton instance of RenderManager
	static RenderManager* getInstance();
	void initScene();
	void renderScene(glm::mat4 viewMatrix, glm::mat4 perspectiveMatrix, glm::vec3 camPos);
	~RenderManager();
private:
	FramebufferObject hdrTexture;
	FramebufferObject pingPong[2];
	ModelOBJ *hdrPlane;
	static RenderManager* pInstance;
	void generateFrameBuffer(GLuint *frameBufferTexture, GLuint *frameBuffer, GLuint *rboDepth, int width, int height);
	void generateFrameBuffer(GLuint *frameBufferTexture, GLuint *frameBuffer, int width, int height);
	void renderFrameBuffer(ModelOBJ *model, GLuint bloomTex);
	void renderBlur(ModelOBJ *model,bool horizontal);
	void renderTexture(ModelOBJ model, glm::mat4 view, glm::mat4 proj, glm::vec3 camPos);
	void renderColor(ModelOBJ model, glm::mat4 view, glm::mat4 proj);
	vector<ModelOBJ> texModels;
	vector<Light> lights;

	GLuint program;
	GLuint programColor;
	GLuint programFramebuffer;
	GLuint programblur;
};