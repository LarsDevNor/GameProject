#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_projection.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class GameManager;

class Camera
{
public:
	Camera();
	virtual ~Camera(void);

	void init();
	void setProjection();

	void rotateX(float deg);
	void rotateY(float deg);
	void moveForward(float dirAndStrength);

	glm::vec3 getEye() const { return eye; }
	glm::mat4 &getViewMatrix() { return viewMatrix; }
	glm::mat4 &getProjMatrix() { return projMatrix; }

	float getFov() const { return fov; }

private:
	void updateView();
	glm::mat4 viewMatrix;
	glm::mat4 projMatrix;

	glm::vec3 eye;
	glm::vec3 rotation;
	glm::vec3 up;

	float moveSpeed;
	float rotateSpeed;

	float fov;
	float near;
	float far;

	glm::ivec2 vpDim;


private:
	GameManager* gm;
};
