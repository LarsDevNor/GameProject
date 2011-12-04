#include "stdafx.h"

#include "Shader.h"
#include "GL/glfw.h"

#include "GameManager.h"
#include "Camera.h"
#include "InputManager.h"


InputManager::InputManager()
{
	prevMouseWheel = glfwGetMouseWheel();
	gm = GameManager::getInstance();
	glfwDisable(GLFW_MOUSE_CURSOR);
}

InputManager::~InputManager()
{
}

void InputManager::handleInput(float dt)
{
	handleKeyboard(dt);
	handleMouse(dt);
}

void InputManager::update(float dt)
{
	handleMouse(dt);
	handleKeyboard(dt);
}

void InputManager::handleMouse(float dt)
{
	glm::ivec2 centerPos;
	glfwGetWindowSize(&centerPos.x,&centerPos.y);
	centerPos /= 2;
	
	glm::ivec2 deltaPos;
	glfwGetMousePos(&deltaPos.x,&deltaPos.y);

	deltaPos -= centerPos;

	glfwSetMousePos(centerPos.x, centerPos.y);
	gm->getActiveCamera()->rotateY(dt*(float)deltaPos.x);
	gm->getActiveCamera()->rotateX(dt*(float)deltaPos.y);
}

void InputManager::handleKeyboard(float dt)
{
	if (glfwGetKey('W'))
	{
		gm->getActiveCamera()->moveForward(dt);
	}
	else if(glfwGetKey('S'))
	{
		gm->getActiveCamera()->moveForward(-dt);
	}

	if(glfwGetKey('A'))
	{
		gm->getActiveCamera()->moveLeft(dt);
	}
	else if(glfwGetKey('D'))
	{
		gm->getActiveCamera()->moveLeft(-dt);
	}	

	if(glfwGetKey(GLFW_KEY_ESC))
		exit(0);

}
