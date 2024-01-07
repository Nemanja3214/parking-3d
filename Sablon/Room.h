#pragma once
#include "Renderable.h"

class Room : Renderable {
public:
	Room(Shader& shader, unsigned int wWidth, unsigned int wHeight);

	using Renderable::setModel;
	using Renderable::getModel;
	using Renderable::setView;
	using Renderable::getView;
	using Renderable::setProjection;
	using Renderable::getProjection;
private:

};