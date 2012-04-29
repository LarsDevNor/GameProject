#pragma once

#include "Camera.h"

class Player
{
public:
	Player();
	~Player();
	void moveForward(float dt);
	void moveLeft(float dt);
	void rotateX(float dt);
	void rotateY(float dt);
	void update(float dt);
	void render();
	Camera & getCamera() {return camera;}
private:
	Camera camera;
	GameManager * gm;
	float moveSpeed;
	float rotateSpeed;
};