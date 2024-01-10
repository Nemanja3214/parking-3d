#include "Car.h"

Car::Car(Shader& shader, glm::mat4 view, glm::mat4 projection)
    : Renderable(shader)
{

    shader.SetUniform3f("uLightPos", glm::vec3(0, 1, 3));
    shader.SetUniform3f("uViewPos", glm::vec3(0, 0, 5));
    shader.SetUniform3f("uLightColor", glm::vec3(1, 1, 1));

    this->model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.125f, 0.125f, 0.125f));
    for (int i = 0; i < 6; ++i) {
        float xOffset = (i % 3) * 2.6f - 2.25f;
        float yOffset = i > 2 ? -9.5f: -4.25f;

        this->models.push_back(glm::translate(model, glm::vec3(xOffset, -5.8f, yOffset + 2.25)));
    }

    shader.SetUniformMat4f("uM", model);

    this->view = view;
    shader.SetUniformMat4f("uV", view);

    this->projection = projection;
    shader.SetUniformMat4f("uP", projection);

}

std::vector<glm::mat4> Car::getModels()
{
    return models;
}
