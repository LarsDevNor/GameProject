#include "stdafx.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_projection.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Camera.h"
#include "GameManager.h"

GameManager::GameManager() : flushError(false), running(true)
{
	gameCamera = new Camera();
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
	
}

void GameManager::render()
{

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

