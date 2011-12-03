#pragma once

/*
	Needs work!

	Lars Hofsøy Breivik
*/

class InputManager
{
public:
	InputManager();
	void handleInput(float dt);
	~InputManager();

private:

	void handleKeyboard(float dt);
	void handleMouse(float dt);

	int prevMouseWheel;
};

