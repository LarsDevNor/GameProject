#include "stdafx.h"

#include "Shader.h"
#include "GL/glfw.h"

#include "TerrainEditor.h"
#include "GameManager.h"
#include "Camera.h"
#include "InputManager.h"
#include "Terrain.h"
#include "Player.h"

InputManager::InputManager()
{
	prevMouseWheel = glfwGetMouseWheel();
	gm = GameManager::getInstance();
	leftMouseButtonHeld 	= false;
	rightMouseButtonJustClicked = false;
	player = gm->getPlayer();
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
	// first person controls    when holding left mouse button 
    // terrain editing          when holding right mouse button 

	if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT))
    {
        glm::ivec2 currentPos;
        glfwGetMousePos(&currentPos.x, &currentPos.y);
        gm->getTED()->run(currentPos);
		leftMouseButtonHeld = true;
    }
	else if ( leftMouseButtonHeld )
	{
		/*glm::ivec2 currentPos;
		glfwGetMousePos(&currentPos.x, &currentPos.y);
		gm->pick(currentPos);*/
		leftMouseButtonHeld = false;
	}

	if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT) && rightMouseButtonJustClicked)
	{
		glm::ivec2 currentPos;
		glfwGetMousePos(&currentPos.x,&currentPos.y);

		glm::ivec2 deltaPos = currentPos - anchorPos;

		glfwSetMousePos(anchorPos.x, anchorPos.y);
		player->rotateY(dt*(float)deltaPos.x);
		player->rotateX(dt*(float)deltaPos.y);
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
		player->moveForward(dt);
	}
	else if(glfwGetKey('S'))
	{
		player->moveForward(-dt);
	}
	
	if(glfwGetKey('A'))
	{
		player->moveLeft(dt);
	}
	else if(glfwGetKey('D'))
	{
		player->moveLeft(-dt);
	}	

    if(glfwGetKey('C'))
    {
        bufferPrev['C'] = 1;
    }
    else if(bufferPrev['C'])
    {
        gm->getTED()->nextConfig();
        bufferPrev['C'] = 0;
    }

	if(glfwGetKey(GLFW_KEY_ESC))
		exit(0);

}
