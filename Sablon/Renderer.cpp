#include "Renderer.h"
#include <iostream>


void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Draw(const VertexArray& va,unsigned int from, unsigned int to, const Shader& shader) const
{
    shader.Bind();
    va.Bind();
    GLCall(GLCall(glDrawArrays(GL_TRIANGLES, from, to)););
}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}
