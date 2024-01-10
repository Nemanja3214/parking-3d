#pragma once
#include "Renderable.h"
#include "Camera.h"



class Room : public Renderable {
public:
	Room(Shader& shader, glm::mat4 view, glm::mat4 projection);

	void setCameras(float direction);

	using Renderable::setModel;
	using Renderable::getModel;
	using Renderable::setView;
	using Renderable::getView;
	using Renderable::setProjection;
	using Renderable::getProjection;
	using Renderable::SetMatrices;
	using Renderable::rotateAroundCenter;
	std::vector<Camera> getCornerCameras();
	bool isCeiling();
	void setCeiling(bool ceiling);
	Camera getBirdCamera();
	float currentAngle;
	int currentCornerCameraIndex;

private:
	std::vector<Camera> cornerCameras;
	Camera birdCamera;
	bool ceiling;
};