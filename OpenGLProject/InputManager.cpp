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
	leftMouseButtonHeld 	= false;
	rightMouseButtonJustClicked = false;
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
	// first person controls when holding left mouse button 

	if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT))
		leftMouseButtonHeld = true;
	else if ( leftMouseButtonHeld )
	{
		glm::ivec2 currentPos;
		glfwGetMousePos(&currentPos.x, &currentPos.y);
		gm->pick(currentPos);
		leftMouseButtonHeld = false;
	}

	if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT) && rightMouseButtonJustClicked)
	{
		glm::ivec2 currentPos;
		glfwGetMousePos(&currentPos.x,&currentPos.y);

		glm::ivec2 deltaPos = currentPos - anchorPos;

		glfwSetMousePos(anchorPos.x, anchorPos.y);
		gm->getActiveCamera()->rotateY(dt*(float)deltaPos.x);
		gm->getActiveCamera()->rotateX(dt*(float)deltaPos.y);
		glfwDisable(GLFW_MOUSE_CURSOR);
	}
	else if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT))
	{
		glfwGetMousePos(&anchorPos.x, &anchorPos.y);
		rightMouseButtonJustClicked = true;
	}
	else
	{
		rightMouseButtonJustClicked = false;
		glfwEnable(GLFW_MOUSE_CURSOR); 
	}
		
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
