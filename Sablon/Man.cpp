#include "Man.h"

Man::Man(Shader& shader, glm::mat4 view, glm::mat4 projection)
    : Renderable(shader)
{

    shader.SetUniform3f("uLightPos", glm::vec3(0, 1, 3));
    shader.SetUniform3f("uViewPos", glm::vec3(0, 0, 5));
    shader.SetUniform3f("uLightColor", glm::vec3(1, 1, 1));

    this->model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.25f, 0.45f, 0.25f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    //model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
   
    model = glm::translate(model, glm::vec3(-2.1f, 2.25f, -1.75f));

    shader.SetUniformMat4f("uM", model);

    this->view = view;
    shader.SetUniformMat4f("uV", view);

    this->projection = projection;
    shader.SetUniformMat4f("uP", projection);

}