
#include "RenderFunctions.h"

void RenderSceneSpiral(Geometry *spiral, GLuint program)
{
	// clear screen to a dark grey colour
	glClearColor(44.0f / 255.0f, 53.0f / 255.0f, 49.0f / 255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	// bind our shader program and the vertex array object containing our
	// scene geometry, then tell OpenGL to draw our geometry
	glUseProgram(program);
	GLint val = glGetUniformLocation(program, "val");
	GLint scale = glGetUniformLocation(program, "scale");
	
	glBindVertexArray(spiral->vertexArray);
	
	
	glUniform1f(scale,1);
	glUniform2f(val,0,0);
	glDrawArrays(GL_LINE_STRIP, 0, spiral->elementCount);  
	
	

      
	
	// reset state to default (no shader or geometry bound)
	glBindVertexArray(0);
	glUseProgram(0);

	// check for an report any OpenGL errors
	CheckGLErrors();
}

void RenderSceneTriangles(Geometry *triangle, GLuint program,int currentLevel)
{
	// clear screen to a dark grey colour
	glClearColor(1, 1, 1, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	// bind our shader program and the vertex array object containing our
	// scene geometry, then tell OpenGL to draw our geometry
	glUseProgram(program);
  	glBindVertexArray(triangle->vertexArray);

	renderTriangle(triangle,program,1,-1,-0.86602540378f,currentLevel -1);

	
	// reset state to default (no shader or geometry bound)
	glBindVertexArray(0);
	glUseProgram(0);

	// check for an report any OpenGL errors
	CheckGLErrors();
}

void renderTriangle(Geometry *triangle,GLuint program,float currentScale,float xPos, float yPos, int currentLevel){
  if(currentLevel == 0){
    GLint scale = glGetUniformLocation(program, "scale");
    GLint posGLSLvar = glGetUniformLocation(program, "mPosition");
    GLint triColor = glGetUniformLocation(program, "triColour");
    glUniform1f(scale,currentScale);
	glm::vec3 leftCornerColor = glm::vec3(xPos*-1 /2,xPos/2,yPos/2);
	glm::vec3 rightCornerColor = glm::vec3((xPos+currentScale)*-1 /2,
	(xPos+currentScale)/2,yPos/2);
	glm::vec3 topCornerColor = glm::vec3((xPos+currentScale/2.0f)*-1 /2,
	(xPos+currentScale/2.0f)/2,(yPos + (1.732050807568877 * currentScale / 2))/2);
	glm::vec3 finalColor = leftCornerColor + rightCornerColor + topCornerColor;
    glUniform2f(posGLSLvar, xPos, yPos);
    glUniform3f(triColor,finalColor.x,finalColor.y,finalColor.z);
    glDrawArrays(GL_TRIANGLES, 0, triangle->elementCount);
  }else{
    renderTriangle(triangle,program,currentScale/2,xPos, yPos,currentLevel -1);
    renderTriangle(triangle,program,currentScale/2,xPos + ((currentScale/2) * 2.0f ),yPos,currentLevel -1);
    renderTriangle(triangle,program,currentScale/2,xPos + (currentScale/2),yPos + (1.732050807568877 * currentScale / 2)
		   ,currentLevel -1);
  }
}

// Rendering function that draws our scene to the frame buffer
void RenderSceneSquares(Geometry *square,Geometry *diamond, GLuint program,int currentLevel)
{
	// clear screen to a dark grey colour
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	// bind our shader program and the vertex array object containing our
	// scene geometry, then tell OpenGL to draw our geometry
	glUseProgram(program);
	GLint val = glGetUniformLocation(program, "val");
	GLint scale = glGetUniformLocation(program, "scale");
	
	glBindVertexArray(square->vertexArray);
	float currentScale = 1.0f;
	float currentVal = 0;
	for(int i =0; i < currentLevel; i++){
	  glUniform1f(scale,currentScale);
	  glUniform2f(val,1-currentVal,currentVal);
	  glDrawArrays(GL_LINE_LOOP, 0, square->elementCount);  
	  currentScale *= 0.5f;
	  currentVal += 0.25f;
	}
	
	// reset state to default (no shader or geometry bound)
	glBindVertexArray(0);

	glBindVertexArray(diamond->vertexArray);
	currentScale = 1.0f;
	currentVal = 0;
	for(int i =0; i < currentLevel; i++){
	  glUniform2f(val,currentVal,0);
	  glUniform1f(scale,currentScale);
	  glDrawArrays(GL_LINE_LOOP, 0, diamond->elementCount);  
	  currentScale *= 0.5f;
	  currentVal += 0.25f;
	}
	
	// reset state to default (no shader or geometry bound)
	glBindVertexArray(0);
	glUseProgram(0);

	// check for an report any OpenGL errors
	CheckGLErrors();
}

void RenderSceneFern(Geometry *fern, GLuint program)
{
	// clear screen to a dark grey colour
	glClearColor(1, 1, 1, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	// bind our shader program and the vertex array object containing our
	// scene geometry, then tell OpenGL to draw our geometry
	glUseProgram(program);
	GLint val = glGetUniformLocation(program, "val");
	
	glBindVertexArray(fern->vertexArray);
	
	glUniform2f(val,0,0);
	glDrawArrays(GL_POINTS, 0, fern->elementCount);  
	
	

      
	
	// reset state to default (no shader or geometry bound)
	glBindVertexArray(0);
	glUseProgram(0);

	// check for an report any OpenGL errors
	CheckGLErrors();
}

void RenderSceneSnowflake(Geometry *snowflake, GLuint program)
{
	// clear screen to a dark grey colour
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	// bind our shader program and the vertex array object containing our
	// scene geometry, then tell OpenGL to draw our geometry
	glUseProgram(program);
	GLint val = glGetUniformLocation(program, "val");
	GLint scale = glGetUniformLocation(program, "scale");
	
	glBindVertexArray(snowflake->vertexArray);
	
	

	glUniform1f(scale,1);
	glUniform2f(val,1,0.5);
	glDrawArrays(GL_LINES, 0, snowflake->elementCount);  
	
	

      
	
	// reset state to default (no shader or geometry bound)
	glBindVertexArray(0);
	glUseProgram(0);

	// check for an report any OpenGL errors
	CheckGLErrors();
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