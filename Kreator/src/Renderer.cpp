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