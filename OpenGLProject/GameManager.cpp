#include "stdafx.h"

#include "glm/gtc/matrix_projection.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Terrain.h"
#include "Camera.h"
#include "InputManager.h"
#include "GameManager.h"

GameManager* GameManager::instance = 0x0;
GameManager* GameManager::getInstance()
{
	if ( !instance )
	{
		instance = new GameManager();
	}
	return instance;
}

GameManager::GameManager() : flushError(false), running(true)
{
	
}

void GameManager::init(const glm::ivec2& _windowDim) 
{
	windowDim = _windowDim;

	cameraDefault = new Camera();
	terrain = new Terrain();
	inputManager = new InputManager();
}

GameManager::~GameManager()
{

}

void GameManager::Tick(float dt)
{
	if(flushError)
		flushGLError();
	update(dt);
	render();
}

void GameManager::update(float dt)
{
	inputManager->update(dt);
}

void GameManager::render()
{
	terrain->render();
}	

void GameManager::flushGLError()
{
	GLenum error = glGetError();
	if ( error != GL_NO_ERROR )
	{
		while ( true )
		{
			if ( error == GL_INVALID_ENUM )
				printf("Invalid enum\n");
			else if ( error == GL_INVALID_VALUE )
				printf("Invalid value\n");
			else if ( error == GL_INVALID_OPERATION )
				printf("Invalid operation\n");
			else if ( error == GL_STACK_OVERFLOW )
				printf("Stack overflow\n");
			else if ( error == GL_STACK_UNDERFLOW )
				printf("Stack underflow\n");
			else if ( error == GL_OUT_OF_MEMORY )
				printf("Out of memory\n");
			//else if ( error == GL_TABLE_TOO_LARGE )
			//	printf("Table too large\n");
			error = glGetError();
			if ( error == GL_NO_ERROR )
				break;
		}
	}
}

