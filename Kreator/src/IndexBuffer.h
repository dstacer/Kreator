#pragma once

class IndexBuffer 
{
private: 
	unsigned int m_RendererId;
	unsigned int m_Count;
public:
	IndexBuffer(const unsigned int* data, unsigned int size);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;
	
	unsigned int GetCount() const { return m_Count; }
};
