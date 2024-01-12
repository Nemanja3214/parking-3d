#include "Car.h"

Car::Car(Shader& shader, glm::mat4 view, glm::mat4 projection)
    : Renderable(shader)
{

    shader.Bind();

    // spotlight 1
    shader.SetUniform1f("uSpotlight1.constant", 2.1f);
    shader.SetUniform1f("uSpotlight1.linear", 0.0f);
    shader.SetUniform1f("uSpotlight1.quadratic", 0.0f);

    shader.SetUniform3f("uSpotlight1.kA", glm::vec3(0.5f, 0.5f, 0.5f));
    shader.SetUniform3f("uSpotlight1.kD", glm::vec3(1.0f, 1.0f, 1.0f));
    shader.SetUniform3f("uSpotlight1.kS", glm::vec3(0.1f, 0.1f, 0.1f));

    // spotlight 2
    shader.SetUniform1f("uSpotlight2.constant", 2.1f);
    shader.SetUniform1f("uSpotlight2.linear", 0.0f);
    shader.SetUniform1f("uSpotlight2.quadratic", 0.0f);

    shader.SetUniform3f("uSpotlight2.kA", glm::vec3(0.5f, 0.5f, 0.5f));
    shader.SetUniform3f("uSpotlight2.kD", glm::vec3(1.0f, 1.0f, 1.0f));
    shader.SetUniform3f("uSpotlight2.kS", glm::vec3(0.1f, 0.1f, 0.1f));

    shader.SetUniform1f("uMaterial.shine", 52);
    shader.SetUniform3f("uMaterial.kA", glm::vec3(0.5f, 0.5f, 0.5f));
    shader.SetUniform3f("uMaterial.kD", glm::vec3(0.5f, 0.5f, 0.5f));
    shader.SetUniform3f("uMaterial.kS", glm::vec3(0.8f, 0.8f, 0.8f));


    this->model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.125f, 0.125f, 0.125f));
    for (int i = 0; i < 6; ++i) {
        float xOffset = (i % 3) * 2.6f - 2.25f;
        float yOffset = i > 2 ? 2.25f : 7.5f;

        this->models.push_back(glm::translate(model, glm::vec3(yOffset + -0.55, -5.8f, xOffset)));
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
