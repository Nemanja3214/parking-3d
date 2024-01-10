#pragma once
#include "Renderable.h"

class Spots : public Renderable {
public:
	Spots(Shader& shader, glm::mat4 view, glm::mat4 projection);

	using Renderable::setModel;
	using Renderable::getModel;
	using Renderable::setView;
	using Renderable::getView;
	using Renderable::setProjection;
	using Renderable::getProjection;
	using Renderable::SetMatrices;
	using Renderable::rotateAroundCenter;

	std::vector<glm::mat4> getModels();


private:
	std::vector<glm::mat4> models;
};