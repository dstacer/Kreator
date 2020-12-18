#pragma once

#include <GL/glew.h>

#include "Shader.h"

class VertexArray;
class IndexBuffer;
class Shader;

#define ASSERT(x) if (!(x)) __debugbreak();
#define GlApiCall(x) ClearGlErrors(); x; ASSERT(LogGlErrors(__FILE__, #x, __LINE__))

void ClearGlErrors();

bool LogGlErrors(const char* file, const char* function, int line);

class Renderer
{	
private:

public:
	Renderer();
	~Renderer();
	void Draw(const VertexArray& vb, const IndexBuffer& ib, const Shader& shader) const;
	void Clear() const;
};
