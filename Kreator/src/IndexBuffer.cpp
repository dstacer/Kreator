#include "Renderer.h"

#include "IndexBuffer.h"


IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    : m_Count(count)
{
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    GlApiCall(glGenBuffers(1, &m_RendererId));

    GlApiCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId));
    GlApiCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    GlApiCall(glDeleteBuffers(1, &m_RendererId));
}

void IndexBuffer::Bind() const
{
    GlApiCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId));
}

void IndexBuffer::Unbind() const
{
    GlApiCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
