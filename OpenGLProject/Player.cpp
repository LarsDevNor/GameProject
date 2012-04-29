#include "StdAfx.h"
#include "Player.h"
#include "Camera.h"
#include "GameManager.h"

Player::Player()
{
	moveSpeed = 4.0;
	rotateSpeed = 1.0;
	gm = GameManager::getInstance();
}


Player::~Player()
{
	
}

void Player::moveForward( float dt )
{
	dt *= moveSpeed;
	camera.moveForward(dt);
}

void Player::moveLeft( float dt )
{
	dt *= moveSpeed;
	camera.moveLeft(dt);
}

void Player::rotateX( float dt )
{
	dt *= rotateSpeed;
	camera.rotateX(dt);
}

void Player::rotateY( float dt )
{
	dt *= rotateSpeed;
	camera.rotateY(dt);
}

void Player::update( float dt )
{

}

void Player::render()
{
	camera.updateView();
}

