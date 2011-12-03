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

	 if(glfwGetKey(GLFW_KEY_LEFT))
	{
		gm->getActiveCamera()->rotateY(-dt);
	}
	else if(glfwGetKey(GLFW_KEY_RIGHT))
	{
		gm->getActiveCamera()->rotateY(dt);
	}
	if(glfwGetKey(GLFW_KEY_UP))
	{
		gm->getActiveCamera()->rotateX(-dt);
	}
	else if(glfwGetKey(GLFW_KEY_DOWN))
	{
		gm->getActiveCamera()->rotateX(dt);
	}
	
}
