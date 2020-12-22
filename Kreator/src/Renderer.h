#pragma once

#include <GL/glew.h>
#include "glm/glm.hpp"

class VertexArray;
class IndexBuffer;
class Shader;

#define ASSERT(x) if (!(x)) __debugbreak();
#define GlApiCall(x) ClearGlErrors(); x; ASSERT(LogGlErrors(__FILE__, #x, __LINE__))

void ClearGlErrors();
bool LogGlErrors(const char* file, const char* function, int line);

namespace Render
{
	const int MAX_QUADS(10000);
	const int MAX_VERTS(4 * MAX_QUADS);
	const int MAX_INDICES(6 * MAX_QUADS);
}


class Renderer
{	
private:

public:
	Renderer();
	~Renderer();
	void Draw(const VertexArray& vb, const IndexBuffer& ib, 
			  const Shader& shader, const unsigned int count = 0) const;
	void Clear() const;
	void SetClearColor(glm::vec4 color);
};
