#pragma once

#include <bitset>

class GameManager;
class Player;
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
	Player * player;


private:
    static const int GAME_KEY_C = 0;
    static const int N_KEYS = 1;
    std::bitset<1024> bufferPrev;
};