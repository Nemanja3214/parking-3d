#pragma once
#include "Renderable.h"

class House : Renderable {
public:
	House(Shader& shader, unsigned int wWidth, unsigned int wHeight);

	using Renderable::setModel;
	using Renderable::getModel;
	using Renderable::setView;
	using Renderable::getView;
	using Renderable::setProjection;
	using Renderable::getProjection;
	using Renderable::SetMatrices;
private:
};