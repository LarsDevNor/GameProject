#include "stdafx.h"

#include <iostream>

#include "GL/glew.h"
#include "GL/glfw.h"

#include "GameManager.h"

int width = 1024;
int height = 768;

GameManager* gm;

bool init();
void initGLState();
void run();

int main(int argc, char** argv)
{
	if (!init())
	{
		std::cerr << "failed to init glfw stuff\n";
		std::cin.get();
		return 1;
	}

	run();

	return 0;
}

bool init()
{
	if (!glfwInit())
	{
		std::cerr << "failed in glfw init\n";
		return false;
	}

	glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);

	if ( !glfwOpenWindow(width, height, 0, 0, 0, 0, 32, 0, /*GLFW_FULLSCREEN*/ GLFW_WINDOW) )
	{
		std::cerr << "Failed when trying to open window\n";
		glfwTerminate();
		return false;
	}

	glfwSetWindowPos(200, 200);

	GLenum e = glewInit();
	if ( e != GLEW_OK )
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(e));
	}

	gm = GameManager::getInstance();

	initGLState();

	return true;
}

void initGLState() 
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
}

void run() 
{
	float prevTime = (float)glfwGetTime(); 
	
	int running = true;

	int frameCount = 0;
	double elapsed = 0.0f;

	while ( running ) 
	{
		float dt = (float)(glfwGetTime() - prevTime);
		prevTime = (float)glfwGetTime();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		gm->Tick(dt);

		glfwSwapBuffers();
		running = glfwGetWindowParam(GLFW_OPENED) & (gm->getRunning() ? 1:0);
	}
}

