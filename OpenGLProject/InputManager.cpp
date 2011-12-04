#include "stdafx.h"

#include "Shader.h"
#include "GL/glfw.h"

#include "Camera.h"
#include "InputManager.h"


InputManager::InputManager()
{
	prevMouseWheel = glfwGetMouseWheel();
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
	//if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT))
	//	sm->getCam()->rotateView(-dt);
	//if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT))
	//	sm->getCam()->rotateView(dt);

	//int mouseWheelNow = glfwGetMouseWheel();
	//int deltaWheel = mouseWheelNow - prevMouseWheel;

	//if ( deltaWheel != 0 )
	//{
	//	sm->getCam()->zoom(deltaWheel);
	//}
	
	//prevMouseWheel = mouseWheelNow;
}

void InputManager::handleKeyboard(float dt)
{

	
	/* cam movement */
	//if (glfwGetKey(GLFW_KEY_ESC))
	//{
	//	sm->setRunning(false);
	//}
	//if (glfwGetKey('W'))
	//	sm->getCam()->moveForward(dt);
	//else if (glfwGetKey('S'))
	//	sm->getCam()->moveForward(-dt);
	//if (glfwGetKey('D'))
	//	sm->getCam()->moveLeft(-dt);
	//else if (glfwGetKey('A'))
	//	sm->getCam()->moveLeft(dt);
	//if (glfwGetKey('R'))
	//	rHeld = true; 
	//else if ( rHeld )
	//{
	//	sm->getTerrainManager()->nextRenderMode();
	//	rHeld = false; 
	//}
	//else if (glfwGetKey('E'))
	//	eHeld = true; 
	//else if (eHeld)
	//{
	//	sm->getTerrainManager()->reloadShader();
	//	eHeld = false;
	//}
	//else if (glfwGetKey('X'))
	//	xHeld = true;
	//else if (xHeld)
	//{
	//	sm->getTerrainManager()->toggleWire();
	//	xHeld = false;
	//}
	//else if (glfwGetKey('P'))
	//	pHeld = true;
	//else if (pHeld)
	//{
	//	sm->getTerrainManager()->toggleVisPatches();
	//	pHeld = false;
	//}
	//else if ( glfwGetKey('F') )
	//	fHeld = true;
	//else if ( fHeld )
	//{
	//	sm->toggleVSync();
	//	fHeld = false;
	//}
	//else if ( glfwGetKey('H') )
	//	hHeld = true;
	//else if ( hHeld ) 
	//{
	//	
	//	sm->printHelp();
	//	hHeld = false;
	//}
	//else if ( glfwGetKey('U') )
	//{
	//	sm->getCam()->rotate(0.1f, glm::vec3(0.0, 1.0, 0.0));
	//}
	//else if ( glfwGetKey('T') )
	//	tHeld = true;
	//else if ( tHeld ) 
	//{
	//	sm->getTerrainManager()->toggleShader();
	//	tHeld = false;
	//}
	//else if ( glfwGetKey('N') )
	//	sm->getTerrainManager()->adjustDetail(0.025f);
	//else if ( glfwGetKey('M') )
	//	sm->getTerrainManager()->adjustDetail(-0.025f);
}
