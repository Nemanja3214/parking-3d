#pragma once
#include "Renderable.h"

class Camera {

public:
	glm::vec3 position;
	glm::vec3 look;
	glm::vec3 up;

	Camera(glm::vec3 position, glm::vec3 look, glm::vec3 up);
	Camera();
	void rotate(float angle);

private:
	glm::mat4 model;
};