#include "stdafx.h"

#include "glm/gtc/matrix_projection.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Terrain.h"
#include "Camera.h"
#include "InputManager.h"
#include "GameManager.h"
#include "PostProcess.h"

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
	initMainFBO();
	ppFog = new PostProcessFog();
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
	terrain->render(sceneFBO);
	ppFog->run(sceneColorTex, sceneDepthTex);
}	

void GameManager::initMainFBO()
{
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &sceneColorTex);
	glGenTextures(1, &sceneDepthTex);
	glGenTextures(1, &sceneNormalTex);

	{ // color rtt target
		glBindTexture(GL_TEXTURE_2D, sceneColorTex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, windowDim.x, windowDim.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	}
	{ // normal rtt target 
		glBindTexture(GL_TEXTURE_2D, sceneNormalTex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, windowDim.x, windowDim.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	}
	{ // depth rtt target 
		glBindTexture(GL_TEXTURE_2D, sceneDepthTex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, windowDim.x, windowDim.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	}

	::flushGLError("GameManager::initTextures() - int rtt textures");

	glGenFramebuffers(1, &sceneFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, sceneFBO);
	glBindTexture(GL_TEXTURE_2D, sceneColorTex);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, sceneColorTex, 0);
	glBindTexture(GL_TEXTURE_2D, sceneNormalTex);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, sceneNormalTex, 0);
	glBindTexture(GL_TEXTURE_2D, sceneDepthTex);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, sceneDepthTex, 0);

	if(::checkFramebuffer(GL_FRAMEBUFFER, "Terrain::initFBO()"))
	{
		// throw or sth 
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	::flushGLError("Terrain::initFBO() - init fbo ");
}