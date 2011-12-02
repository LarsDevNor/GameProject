#include "stdafx.h"

#include <iostream>

#include "GL/glew.h"
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

Camera::Camera() : moveSpeed(0.25f), zoomSpeed(0.0275f)
{
	init();
}

Camera::~Camera()
{
}

void Camera::init()
{
	fov = 70.0f;
	near = 0.1f;
	far = 100.0f;
//TODO	glViewport(0, 0, vpWidth, vpHeight);
	setProjection();
	viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	dRot = 0.0f;
	rotateSpeed = 1.0f;
}

void Camera::lookAt(const glm::vec3 &_eye, const glm::vec3 &_target, const glm::vec3 &_up)
{
	eye = _eye;
	target = _target;
	up = _up;

	lookAt();
}

void Camera::lookAt()
{
	glm::vec3 zAxis = target - eye;
	zAxis = glm::normalize(zAxis);

	glm::vec3 xAxis = glm::cross(zAxis, up);
	xAxis = glm::normalize(xAxis);

	glm::vec3 yAxis = glm::cross(xAxis, zAxis);
	yAxis = glm::normalize(yAxis);

	up = yAxis;

	viewMatrix[0][0] = xAxis.x;
	viewMatrix[1][0] = xAxis.y;
	viewMatrix[2][0] = xAxis.z;

	viewMatrix[0][1] = yAxis.x;
	viewMatrix[1][1] = yAxis.y;
	viewMatrix[2][1] = yAxis.z;

	viewMatrix[0][2] = -zAxis.x;
	viewMatrix[1][2] = -zAxis.y;
	viewMatrix[2][2] = -zAxis.z;

	viewMatrix[3][0] = 0.0f;
	viewMatrix[3][1] = 0.0f;
	viewMatrix[3][2] = 0.0f;
	
	viewMatrix[0][3] = 0.0f;
	viewMatrix[1][3] = 0.0f;
	viewMatrix[2][3] = 0.0f;
	viewMatrix[3][3] = 1.0f;

	glm::mat4 translMat = glm::translate(glm::mat4(1.0), -eye);

	viewMatrix = viewMatrix * translMat;
}

void Camera::setProjection()
{
//TODO	float aspect = (float)sm->getWidth() / (float)sm->getHeight();
//TODO	projMatrix = glm::perspective(fov, aspect, near, far);
}

void Camera::rotate(float deg, const glm::vec3& axis)
{
	viewMatrix = glm::rotate(viewMatrix, deg, axis);
}

void Camera::moveLeft(float dirAndStrength)
{
	glm::vec3 zAxis = target - eye;
	zAxis.y = 0;
	zAxis = glm::normalize(zAxis);

	glm::vec3 xAxis = glm::cross(zAxis, up);
	xAxis.y = 0;
	xAxis = glm::normalize(xAxis);

	up = glm::vec3(0.0f, 1.0f, 0.0f);

	eye -= xAxis*moveSpeed*dirAndStrength;
	target -= xAxis*moveSpeed*dirAndStrength;

	lookAt();
}

float someVar = 0.001;
void Camera::setTarget(const glm::vec3& _target)
{
	target = _target;

	lookAt();
}

void Camera::rotateView(float dirAndSpeed)
{
	dRot += dirAndSpeed*rotateSpeed;
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	target.x = eye.x + sin(dRot);
	target.z = eye.z + cos(dRot);
		
	lookAt();
}

void Camera::moveForward(float dirAndStrength)
{
	glm::vec3 zAxis = target - eye;
	zAxis.y = 0;
	zAxis = glm::normalize(zAxis);

	glm::vec3 xAxis = glm::cross(zAxis, up);
	xAxis.y = 0;
	xAxis = glm::normalize(xAxis);

	eye += zAxis * dirAndStrength * moveSpeed;
	target += zAxis * dirAndStrength * moveSpeed;

	lookAt();
}

void Camera::zoom(int amount)
{
	eye.y -= zoomSpeed * amount * eye.y*0.8f;
	lookAt();
}
