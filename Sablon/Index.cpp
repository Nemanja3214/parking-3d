#include "Index.h"
#include <glm/gtx/transform.hpp>

Index::Index(Shader& shader, glm::mat4 view, glm::mat4 projection)
    : Renderable(shader)
{

    this->model = glm::mat4(1.0f);
    //model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
   
    glm::vec3 translation = glm::vec3(-0.12f, -0.2f, 1.7f);
    model = glm::translate(model, translation);
    model = glm::scale(model, glm::vec3(0.065f, 0.065f, 0.065f));

    shader.SetUniformMat4f("uM", model);

    this->view = view;
    shader.SetUniformMat4f("uV", view);

    this->projection = projection;
    shader.SetUniformMat4f("uP", projection);

}