#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"

VertexArray::VertexArray()
{
	GlApiCall(glGenVertexArrays(1, &m_RendererId));
}

VertexArray::~VertexArray()
{
	GlApiCall(glDeleteVertexArrays(1, &m_RendererId));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& vblayout)
{
	Bind();
	vb.Bind();

	const auto& elements = vblayout.GetElements();
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GlApiCall(glEnableVertexAttribArray(i));
		GlApiCall(glVertexAttribPointer(i, element.count, element.type, element.normalized,
										vblayout.GetStride(), (const void*)element.offset));
	}
	
}

void VertexArray::Bind() const
{
	GlApiCall(glBindVertexArray(m_RendererId));
}

void VertexArray::Unbind() const
{
	GlApiCall(glBindVertexArray(0));
}
