#pragma once

#include <vector>
#include <GL/glew.h>

struct VBElement
{
	unsigned int count;
	GLenum type;
	GLboolean normalized;
	unsigned int offset;

};

class VertexBufferLayout
{
private:
	std::vector<VBElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout()
		: m_Stride(0) { }

	// unmatched type spec
	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(false);
	}

	template<> 
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back( { count, GL_FLOAT, GL_FALSE, m_Stride } );
		m_Stride += count * sizeof(GLfloat);
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back( { count, GL_UNSIGNED_INT, GL_FALSE, m_Stride } );
		m_Stride += count * sizeof(GLuint);
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back( { count, GL_UNSIGNED_BYTE, GL_TRUE, m_Stride } );
		m_Stride += count * sizeof(GLubyte);
	}

	const unsigned int GetStride() const { return m_Stride; }

	const std::vector<VBElement>& GetElements() const {
		return m_Elements;
	}
};