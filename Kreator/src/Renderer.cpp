#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Renderer.h"

#include <iostream>

void ClearGlErrors()
{
    while (glGetError() != GL_NO_ERROR);
}

bool LogGlErrors(const char* file, const char* function, int line)
{
    while (GLenum err = glGetError())
    {
        std::cout << "OPENGL ERROR: (0x" << std::hex << err << ") in "
            << function << " " << file << ": " << std::dec << line << std::endl;
        return false;
    }
    return true;
}

Renderer::Renderer()
{
    GlApiCall(glEnable(GL_BLEND));
    GlApiCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

Renderer::~Renderer()
{
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, 
                    const Shader& shader, const unsigned int count) const
{
    shader.Bind();

    va.Bind();
    ib.Bind();
    unsigned int indexCount = count == 0 ? ib.GetCount() : count;
    GlApiCall(glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clear() const
{
    GlApiCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::SetClearColor(glm::vec4 color)
{
    GlApiCall(glClearColor(color.r, color.g, color.b, color.a));
}
