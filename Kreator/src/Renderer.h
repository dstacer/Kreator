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

struct Vertex
{
	float Position[3];
	float Color[4];
	float TexCoords[2];
	float TexUnit;
};

class Renderer
{	
private:

public:
	Renderer();
	~Renderer();
	void Draw(const VertexArray& vb, const IndexBuffer& ib, const Shader& shader) const;
	void Clear() const;
	void SetClearColor(glm::vec4 color);
};
