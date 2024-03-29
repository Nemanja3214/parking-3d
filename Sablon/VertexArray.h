#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Debug.h"

class VertexArray
{
private:
    unsigned int m_RendererID;

public:
    VertexArray();
    ~VertexArray();

    void AddBuffer(VertexBuffer& vb, const VertexBufferLayout& layout);
    void Bind() const;
    void Unbind() const;
};