#include "Room.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>


Room::Room(Shader& shader, unsigned int wWidth, unsigned int wHeight, glm::mat4 view, glm::mat4 projection)
    : Renderable(shader)
{

    this->model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));

    shader.SetUniformMat4f("uM", model);

    this->view = view;
    shader.SetUniformMat4f("uV", view);

    this->projection = projection;
    shader.SetUniformMat4f("uP", projection);


}

