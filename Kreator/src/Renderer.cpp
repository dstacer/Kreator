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

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    shader.Bind();
    //shaderProgram.SetUniform4f("u_color", r, 0.2f, 0.8f, 1.0f);

    va.Bind();
    ib.Bind();
    GlApiCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clear() const
{
    GlApiCall(glClear(GL_COLOR_BUFFER_BIT));
}
