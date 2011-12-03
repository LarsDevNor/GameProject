#pragma once

#include <vector>
#include <iostream>

#include "GL/glfw.h"

class Camera;

class GameManager
{
public:
	GameManager();
	virtual ~GameManager();

	void Tick(float dt);
	void toggleFlushError() { flushError = !flushError; printf("Flusing GLError: %i\n", flushError); }
	void toggleVSync() { vSync = !vSync; vSync ? glfwSwapInterval(1) : glfwSwapInterval(0); }
	void setRunning(bool _running) { running = _running; }
	bool getRunning() const { return running; }

private:
	void update(float dt);
	void render();
	void flushGLError();

	bool flushError;
	bool vSync;
	bool running;

	Camera* gameCamera;
	Terrain* terrain;
};

