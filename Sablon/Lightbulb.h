#pragma once
#include "Renderable.h"

class Lightbulb : public Renderable {
public:
	Lightbulb(Shader& shader, glm::mat4 view, glm::mat4 projection);

	using Renderable::setModel;
	using Renderable::getModel;
	using Renderable::setView;
	using Renderable::getView;
	using Renderable::setProjection;
	using Renderable::getProjection;
	using Renderable::SetMatrices;
	using Renderable::rotateAroundCenter;

private:
};