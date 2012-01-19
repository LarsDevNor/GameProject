#pragma once

#include <vector>
#include <iostream>

#include "GL/glfw.h"

class Camera;
class Terrain;
class InputManager;
class PostProcessFog;

class GameManager
{
public:
	static GameManager* getInstance();
public:
	void init(const glm::ivec2& _windowDim);
	virtual ~GameManager();
	void Tick(float dt);
	void toggleFlushError() { flushError = !flushError; (flushError) ? printf("Flusing GLError") : printf("Stopped Flusing GLError"); }
	void toggleVSync() { vSync = !vSync; vSync ? glfwSwapInterval(1) : glfwSwapInterval(0); }
public:
	void setRunning(bool _running) { running = _running; }
	bool getRunning() const { return running; }
	Camera* getActiveCamera() { return cameraDefault; }
	void setWindowDim(const glm::ivec2& _windowDim) { windowDim = _windowDim; }
	glm::ivec2 getWindowDim () const { return windowDim; }

private:
	static GameManager* instance;
private:
	GameManager();
	void update(float dt);
	void render();
	void initMainFBO(); 
	bool flushError;
	bool vSync;
	bool running;
	Camera* cameraDefault;
	Terrain* terrain;
	InputManager* inputManager;
	glm::ivec2 windowDim;

	GLuint sceneColorTex;
	GLuint sceneDepthTex;
	GLuint sceneNormalTex;
	GLuint sceneFBO;

	PostProcessFog* ppFog;
};

