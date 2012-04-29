#pragma once

class Camera;
class Terrain;
class InputManager;
class PostProcessFog;
class PostProcessSSAO;
class TerrainEditor;
class CLManager;
class ParticleSystem;
class Player;

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
	void setRunning(bool _running) 	{ running = _running; }
	bool getRunning() const 		{ return running; }
	Camera* getActiveCamera();
	void setWindowDim(const glm::ivec2& _windowDim) { windowDim = _windowDim; }
	glm::ivec2 getWindowDim () const { return windowDim; }
    Terrain* getTerrain() const { return terrain; }
    TerrainEditor* getTED() const { return ted; }
	Player * getPlayer() const {return player; }

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

	Player * player;
	Terrain* terrain;
    TerrainEditor* ted;
    CLManager* clManager;
	InputManager* inputManager;
	glm::ivec2 windowDim;

	Texture sceneColorTex;
	Texture sceneDepthTex;
	Texture sceneNormalTex;
	Texture scenePositionTex;
	Texture scenePickingTex;
	GLuint sceneFBO;

	PostProcessFog* ppFog;
	PostProcessSSAO* ppSSAO;

    ParticleSystem* particleSystem;
};

