#pragma once

#include <GL/glew.h>
#include <assert.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Renderer
{
public:
    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void DrawLineStrip(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void Draw(const VertexArray& va, unsigned int from, unsigned int to, const Shader& shader) const;
    void DrawLineStrip(const VertexArray& va, unsigned int from, unsigned int to, const Shader& shader) const;
    void DrawFan(const VertexArray& va, unsigned int from, unsigned int to, const Shader& shader) const;
};