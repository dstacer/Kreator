#pragma once

//#include <array>

#include "Renderer.h"

class VertexBuffer;
class VertexBufferLayout;
struct Vertex; 

class VertexArray
{
private:
	uint32_t m_RendererId;

public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vbuf, const VertexBufferLayout& vblayout);
	Vertex* CreateQuad(Vertex* target, glm::vec3 pos, glm::vec2 size, float texId);

	void Bind() const;
	void Unbind() const;
};


