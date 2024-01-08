#include "Renderable.h"
#include "VertexArray.h"


Renderable::Renderable(Shader& shader, glm::mat4& model, glm::mat4& view, glm::mat4& projection)
    : shader(shader), model(model), view(view), projection(projection)
{
    shader.Bind();
    shader.SetUniformMat4f("uM", model);
    shader.SetUniformMat4f("uV", view);
    shader.SetUniformMat4f("uP", projection);
    //shader.Unbind();
}
Renderable::Renderable(Shader& shader)
    : shader(shader)
{
}

void Renderable::SetMatrices()
{
    shader.Bind();
    shader.SetUniformMat4f("uM", model);
    shader.SetUniformMat4f("uV", view);
    shader.SetUniformMat4f("uP", projection);
}

//Renderable::~Renderable()
//{
    
//}


void Renderable::setModel(glm::mat4 modelV) {
    model = modelV;
    shader.Bind();
    shader.SetUniformMat4f("uM", model);
    //shader.Unbind();
}
// Getter
glm::mat4 Renderable::getModel() {
    return model;
}

void Renderable::setView(glm::mat4 viewV) {
    view = viewV;
    shader.Bind();
    shader.SetUniformMat4f("uV", view);
    //shader.Unbind();
}
// Getter
glm::mat4 Renderable::getView() {
    return view;
}

void Renderable::setProjection(glm::mat4 projectionV) {
    projection = projectionV;
    shader.Bind();
    shader.SetUniformMat4f("uP", projection);
    //shader.Unbind();
}
// Getter
glm::mat4 Renderable::getProjection() {
    return projection;
}
