#pragma once

#include "glm/glm.hpp"

class VertexBuffer 
{
private: 
	unsigned int m_RendererId;

public:
	VertexBuffer();
	VertexBuffer(void* data, unsigned int size);
	
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
	void Fill(const void* data, unsigned int size) const;
	
};
