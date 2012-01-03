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
		::flushGLError("GameManager::Tick()");
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



