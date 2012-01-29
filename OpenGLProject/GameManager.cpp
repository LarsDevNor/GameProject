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
	ppSSAO = new PostProcessSSAO();
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
	//terrain->render(sceneFBO); // prepare for post processing 
	terrain->render(0); // to screen 

	//ppSSAO->run(sceneColorTex, sceneNormalTex, scenePositionTex, sceneDepthTex);
	//ppFog->run(sceneColorTex, sceneDepthTex); 
}	

void GameManager::pick(const glm::ivec2& screenCoord)
{
	terrain->pick(screenCoord);
}

void GameManager::initMainFBO()
{
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &sceneColorTex);
	glGenTextures(1, &sceneDepthTex);
	glGenTextures(1, &sceneNormalTex);
	glGenTextures(1, &scenePickingTex);

	{ // color rtt target
		glBindTexture(GL_TEXTURE_2D, sceneColorTex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, windowDim.x, windowDim.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	}
	{ // normal rtt target 
		glBindTexture(GL_TEXTURE_2D, sceneNormalTex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, windowDim.x, windowDim.y, 0, GL_RGBA, GL_FLOAT, 0);
	}
	{ // position rtt target 
		glBindTexture(GL_TEXTURE_2D, scenePositionTex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, windowDim.x, windowDim.y, 0, GL_RGBA, GL_FLOAT, 0);
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
	{ // picking ID target 
		glBindTexture(GL_TEXTURE_2D, scenePickingTex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, windowDim.x, windowDim.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);	
	}

	::flushGLError("GameManager::initTextures() - int rtt textures");

	glGenFramebuffers(1, &sceneFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, sceneFBO);
	glBindTexture(GL_TEXTURE_2D, sceneColorTex);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, sceneColorTex, 0);
	glBindTexture(GL_TEXTURE_2D, sceneNormalTex);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, sceneNormalTex, 0);
	glBindTexture(GL_TEXTURE_2D, scenePositionTex);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, scenePositionTex, 0);
	glBindTexture(GL_TEXTURE_2D, scenePickingTex);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, scenePickingTex, 0);
	glBindTexture(GL_TEXTURE_2D, sceneDepthTex);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, sceneDepthTex, 0);

	::checkFramebuffer(GL_FRAMEBUFFER, "Terrain::initFBO()");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	::flushGLError("Terrain::initFBO() - init fbo ");
}