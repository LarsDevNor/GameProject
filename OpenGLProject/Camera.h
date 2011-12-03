#if 1

/*
	Simple camera 

	Lars Hofsøy Breivik
*/

#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_projection.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Camera
{
public:
	Camera();
	virtual ~Camera(void);

	void init();
	void lookAt();
	void lookAt(const glm::vec3 &eye, const glm::vec3 &target, const glm::vec3 &up);
	void setProjection();
	void setTarget(const glm::vec3& target);
	void rotate(float deg, const glm::vec3& axis);
	
	void moveLeft(float dirAndStrength);
	//void moveRight();
	void moveForward(float dirAndStrength);
	//void moveBackward();
	void zoom(int amount);
	void rotateView(float dirAndStrength);

	glm::vec3 getEye() const { return eye; }
	glm::vec3 getTarget() const { return target; }
	glm::mat4 &getViewMatrix() { return viewMatrix; }
	glm::mat4 &getProjMatrix() { return projMatrix; }

	float getFov() const { return fov; }

private:
	glm::mat4 viewMatrix;
	glm::mat4 projMatrix;

	glm::vec3 eye;
	glm::vec3 target;
	glm::vec3 up;

	float moveSpeed;
	float rotateSpeed;
	float dRot;

	float zoomSpeed;
	float fov;
	float near;
	float far;

	size_t vpWidth;
	size_t vpHeight;
};

#endif 