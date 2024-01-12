#include "Ramp.h"
#include <glm/gtx/transform.hpp>

Ramp::Ramp(Shader& shader, glm::mat4 view, glm::mat4 projection)
    : Renderable(shader)
{
    shader.Bind();

    shader.SetUniform1i("uIsEmission", 0);

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
    shader.SetUniform3f("uMaterial.kS", glm::vec3(0.5f, 0.5f, 0.5f));

    this->model = glm::mat4(1.0f);
    //model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.85f, 0.45f, 0.15f));
    glm::vec3 translation = glm::vec3(0.0f, -1.25f, 2.5f);
    model = glm::translate(model, translation);

    shader.SetUniformMat4f("uM", model);

    this->view = view;
    shader.SetUniformMat4f("uV", view);

    this->projection = projection;
    shader.SetUniformMat4f("uP", projection);

    glm::vec4 coorStart = glm::vec4(0.0f);
    coorStart[3] = 1;
    this->rampCenter = glm::translate(glm::mat4(1.0), translation) *coorStart;
}

glm::mat4 Ramp::rotate() {
    //possibly interfer with this->model
    //glm::mat4 modelV = glm::mat4(1.0f);
    //modelV = glm::scale(modelV, glm::vec3(0.85f, 0.45f, 0.15f));
    //glm::vec3 negatedRampCenter = -rampCenter;
    //glm::mat4 modelV = glm::translate(model, negatedRampCenter);
    glm::mat4 modelV = glm::rotate(model, glm::radians(rampAngle), glm::vec3(0.0f, 0.0f, 1.0f));
    //modelV = glm::translate(modelV, rampCenter);
    return modelV;
    //shader.SetUniformMat4f("uM", model);
}