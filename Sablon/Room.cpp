#include "Room.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>


Room::Room(Shader& shader, unsigned int wWidth, unsigned int wHeight)
    : Renderable(shader)
{

    this->model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));

    shader.SetUniformMat4f("uM", model);

    this->view = glm::mat4(1.0f);
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    shader.SetUniformMat4f("uV", view);

    this->projection = glm::perspective(glm::radians(90.0f), (float)wWidth / (float)wHeight, 0.1f, 10.0f);
    shader.SetUniformMat4f("uP", projection);


}

