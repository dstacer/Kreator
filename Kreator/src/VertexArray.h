#pragma once
#include <array>

class VertexBuffer;
class VertexBufferLayout;
struct Vertex;
struct Vec2;

class VertexArray
{
private:
	unsigned int m_RendererId;

public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& vblayout);
    Vertex * CreateQuad(Vertex * target, Vec2 pos, Vec2 size, float texId);

	void Bind() const;
	void Unbind() const;
};


