#include "Camera.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 look, glm::vec3 up) : position(position), look(look), up(up){
	this->model = glm::mat4(1.0f);
}
Camera::Camera(){
	this->model = glm::mat4(1.0f);
}

void Camera::rotate(float angle) {
	//glm::mat4 cameraModel = glm::translate(glm::mat4(1.0f), -position);
	this->model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	//cameraModel = glm::translate(cameraModel, position);

	//position = cameraModel * glm::vec4(position, 1.0);
	look = glm::vec3(model * glm::vec4(look, 1.0));
	up = glm::vec3(model * glm::vec4(up, 1.0));

};
