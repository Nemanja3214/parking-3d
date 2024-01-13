#include "Lightbulb.h"

Lightbulb::Lightbulb(Shader& shader, glm::mat4 view, glm::mat4 projection)
    : Renderable(shader)
{

    shader.Bind();

    shader.SetUniform3f("uLight.kA", glm::vec3(0.5f, 0.5f, 0.5f));
    shader.SetUniform3f("uLight.kD", glm::vec3(1.0f, 1.0f, 0.3f));
    shader.SetUniform3f("uLight.kS", glm::vec3(0.5f, 0.5f, 0.5f));

    shader.SetUniform1f("uMaterial.shine", 52);
    shader.SetUniform3f("uMaterial.kA", glm::vec3(0.7f, 0.7f, 0.7f));
    shader.SetUniform3f("uMaterial.kD", glm::vec3(1.0f, 1.0f, 1.0f));
    shader.SetUniform3f("uMaterial.kS", glm::vec3(0.7f, 0.7f, 0.7f));

    shader.SetUniform1f("uLight.constant", 1.0f);
    shader.SetUniform1f("uLight.linear", 0.19f);
    shader.SetUniform1f("uLight.quadratic", 0.0f);

    this->model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
   // model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    //model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    model = glm::translate(model, glm::vec3(-2.1f, -2.3f, 1.8f));

    shader.SetUniformMat4f("uM", model);

    this->view = view;
    shader.SetUniformMat4f("uV", view);

    this->projection = projection;
    shader.SetUniformMat4f("uP", projection);

}