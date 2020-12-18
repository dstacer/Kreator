#include "Renderer.h"

#include "VertexBuffer.h"


VertexBuffer::VertexBuffer(void* data, unsigned int size)
{
    GlApiCall(glGenBuffers(1, &m_RendererId));
    GlApiCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererId));
    GlApiCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    GlApiCall(glDeleteBuffers(1, &m_RendererId));
}

void VertexBuffer::Bind() const
{
    GlApiCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererId));
}

void VertexBuffer::Unbind() const
{
    GlApiCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
