#pragma once
#include "Shader.h"
//#include "Renderer.h"
#include "VertexArray.h"
class Renderable {
protected:
	Shader shader;
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
public:
	Renderable(Shader& shader, glm::mat4& model, glm::mat4& view, glm::mat4& projection);
	Renderable(Shader& shader);

	void SetMatrices();

	void setModel(glm::mat4 model);
	glm::mat4 getModel();
	void setView(glm::mat4 view);
	glm::mat4 getView();
	void setProjection(glm::mat4 projection);
	glm::mat4 getProjection();

};
