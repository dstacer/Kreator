#pragma once

class VertexBuffer;
class VertexBufferLayout;

class VertexArray
{
private:
	unsigned int m_RendererId;

public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& vblayout);

	void Bind() const;
	void Unbind() const;
};