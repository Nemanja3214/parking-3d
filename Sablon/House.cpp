#include "House.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>



House::House(Shader& shader, glm::mat4 view, glm::mat4 projection)
    : Renderable(shader)
{

    this->model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.4f, 1.0f, 0.25f));
    model = glm::translate(model, glm::vec3(1.0f, -0.25f, 2.5f));

    shader.SetUniformMat4f("uM", model);

    this->view = view;
    shader.SetUniformMat4f("uV", view);

    this->projection = projection;
    shader.SetUniformMat4f("uP", projection);

    houseCamera = { glm::vec3(model * glm::vec4(0.0f, 0.5f, 0.0f, 1.0f)), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f) };
}

Camera House::getHouseCamera()
{
    return this->houseCamera;
}

