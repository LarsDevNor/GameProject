#include "stdafx.h"

#include <iostream>

#include "GL/glew.h"

#include "GameManager.h"
#include "Camera.h"

#define PI 3.14159265

void printMatrix(const glm::mat4& mat)
{
	printf("%.2g\t%.2g\t%.2g\t%.2g\n%.2g\t%.2g\t%.2g\t%.2g\n%.2g\t%.2g\t%.2g\t%.2g\n%.2g\t%.2g\t%.2g\t%.2g\n", 
		mat[0][0], mat[0][1], mat[0][2], mat[0][3],
		mat[1][0], mat[1][1], mat[1][2], mat[1][3], 
		mat[2][0], mat[2][1], mat[2][2], mat[2][3],
		mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
}

Camera::Camera()
{
	gm = GameManager::getInstance();
	init();
}

Camera::~Camera()
{
}

void Camera::init()
{
	vpDim = gm->getWindowDim();
	rotateSpeed = 5.0f;
	moveSpeed = 50.0f;
	fov = 60.0f;
	nearPlane = 1.0f;
	farPlane = 5000.0f;
	glViewport(0, 0, vpDim.x, vpDim.y);
	setProjection();

	eye = glm::vec3(0.0f, 0.0f, 0.0f);
	rotation = glm::vec3(20.0f, 135.0f, 0.0f);

	updateView();

	printMatrix(viewMatrix);
	
}

void Camera::setProjection()
{
	float aspect =  (float)vpDim.x / vpDim.y;
	projMatrix = glm::perspective(fov, aspect, nearPlane, farPlane);
}


void Camera::moveForward(float dt)
{
	glm::vec3 forward(viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2]);
	eye += dt * moveSpeed * forward;
	updateView();
}

void Camera::moveLeft(float dt)
{
	glm::vec3 left(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
	eye += left * dt * moveSpeed;
	updateView();
}

void Camera::updateView()
{
	viewMatrix = glm::rotate(glm::mat4(1.0), rotation[0], glm::vec3(1.0f, 0.0f, 0.0f));
	viewMatrix = glm::rotate(viewMatrix,	 rotation[1], glm::vec3(0.0f, 1.0f, 0.0f));
	viewMatrix = glm::rotate(viewMatrix,     rotation[2], glm::vec3(0.0f, 0.0f, 1.0f));
	viewMatrix = glm::translate(viewMatrix,eye);
}

void Camera::rotateY(float dt)
{
	rotation[1] += dt * rotateSpeed;
	if(rotation[1] > 360.0f)
		rotation[1] -= 360.0f;
	else if(rotation[1] <= -360.0f)
		rotation[1] += 360.0f;

	updateView();
}

void Camera::rotateX(float dt)
{
	rotation[0] += dt * rotateSpeed;
	if(rotation[0] > 360.0f)
		rotation[0] -= 360.0f;
	else if(rotation[0] <= -360.0f)
		rotation[0] += 360.0f;

	updateView();
}
