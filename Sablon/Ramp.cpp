#include "Ramp.h"
#include <glm/gtx/transform.hpp>

Ramp::Ramp(Shader& shader, unsigned int wWidth, unsigned int wHeight, glm::mat4 view, glm::mat4 projection)
    : Renderable(shader)
{

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