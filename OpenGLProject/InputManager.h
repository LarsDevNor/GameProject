#pragma once

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

	int prevMouseWheel;
};

