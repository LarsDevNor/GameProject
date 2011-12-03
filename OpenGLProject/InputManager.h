#pragma once

/*
	Handles input 
	Silly with stupid bools etc, but its not really all that important

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

