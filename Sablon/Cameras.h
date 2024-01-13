#pragma once
#include "Renderable.h"
#include "Camera.h"


class Cameras : public Renderable {
public:

	using Renderable::setModel;
	using Renderable::getModel;
	using Renderable::setView;
	using Renderable::getView;
	using Renderable::setProjection;
	using Renderable::getProjection;
	using Renderable::SetMatrices;
	using Renderable::rotateAroundCenter;


	Cameras(Shader& shader, glm::mat4 view, glm::mat4 projection, std::vector<Camera> cameras);

	void setCameras(float direction);

	std::vector<glm::mat4> getModels();


private:
	std::vector<glm::mat4> models;
};