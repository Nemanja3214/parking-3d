#pragma once
#include "Renderable.h"
#include "Room.h"

class House : public Renderable {
public:
	House(Shader& shader, unsigned int wWidth, unsigned int wHeight, glm::mat4 view, glm::mat4 projection);

	using Renderable::setModel;
	using Renderable::getModel;
	using Renderable::setView;
	using Renderable::getView;
	using Renderable::setProjection;
	using Renderable::getProjection;
	using Renderable::SetMatrices;
	using Renderable::rotateAroundCenter;

	Camera getHouseCamera();
private:
	Camera houseCamera;
};