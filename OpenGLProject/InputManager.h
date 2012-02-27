#pragma once

class GameManager;

class InputManager
{
public:
 	InputManager();
 	void handleInput(float dt);
 	void update(float dt);
 	~InputManager();
private:
 	void handleKeyboard(float dt);
 	void handleMouse(float dt);
 private:
 	std::vector<unsigned int> keys;
 	int prevMouseWheel;
    GameManager *gm;
    bool leftMouseButtonHeld;
    bool rightMouseButtonJustClicked;
    glm::ivec2 anchorPos;
};