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

	void rotateX(float dt);
	void rotateY(float dt);
	void moveForward(float dt);
	void moveLeft(float dt);

	glm::vec3 getEye() const { return eye; }
	glm::mat4 &getViewMatrix() { return viewMatrix; }
	glm::mat4 &getProjMatrix() { return projMatrix; }

	float getFov() const { return fov; }
	void updateView();

private:
	
	glm::mat4 viewMatrix;
	glm::mat4 projMatrix;

	glm::vec3 eye;
	glm::vec3 rotation;
	glm::vec3 up;

	float moveSpeed;
	float rotateSpeed;

	float fov;
	float nearPlane;
	float farPlane;

	glm::ivec2 vpDim;


private:
	GameManager* gm;
};
