#include "stdafx.h"

#include "glm/gtc/matrix_projection.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ParticleSystem.h"
#include "CLManager.h"
#include "Terrain.h"
#include "TerrainEditor.h"
#include "Camera.h"
#include "InputManager.h"
#include "GameManager.h"
#include "PostProcess.h"
#include "Player.h"

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
    
    clManager = CLManager::getInstance();

//	cameraDefault = new Camera();
	player = new Player();
	terrain = new Terrain();
    ted = new TerrainEditor(terrain);
	inputManager = new InputManager();
	initMainFBO();
	ppFog = new PostProcessFog();
	ppSSAO = new PostProcessSSAO();
    particleSystem = new ParticleSystem();
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

    particleSystem->render();
}	

void GameManager::initMainFBO()
{
    sceneColorTex = Texture(windowDim, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, GL_LINEAR, GL_CLAMP_TO_EDGE, 0);
    {
        sceneDepthTex = Texture(windowDim, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, GL_NEAREST, GL_CLAMP_TO_EDGE, 0);
        sceneDepthTex.bind();
        glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    }
	sceneNormalTex = Texture(windowDim, GL_RGBA16, GL_RGBA, GL_FLOAT, GL_LINEAR, GL_CLAMP_TO_EDGE, 0);
	scenePickingTex = Texture(windowDim, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, GL_LINEAR, GL_CLAMP_TO_EDGE, 0);
    scenePositionTex = Texture(windowDim, GL_RGBA16, GL_RGBA, GL_FLOAT, GL_LINEAR, GL_CLAMP_TO_EDGE, 0);

	::flushGLError("GameManager::initTextures() - int rtt textures");

	glGenFramebuffers(1, &sceneFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, sceneFBO);
    sceneColorTex.bind();
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, sceneColorTex.get(), 0);
	sceneNormalTex.bind();
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, sceneNormalTex.get(), 0);
    scenePositionTex.bind();
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, scenePositionTex.get(), 0);
    scenePickingTex.bind();
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, scenePickingTex.get(), 0);
	sceneDepthTex.bind();
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, sceneDepthTex.get(), 0);

	::checkFramebuffer(GL_FRAMEBUFFER, "Terrain::initFBO()");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	::flushGLError("GameManager::initFBO() - init fbo ");
}

Camera* GameManager::getActiveCamera()
{
	return &player->getCamera();
}
