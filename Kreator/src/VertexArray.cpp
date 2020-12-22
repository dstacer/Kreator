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

Vertex * VertexArray::CreateQuad(Vertex* target, Vec2 pos, Vec2 size, float texId)
{
	//Vertex v1;
	target->Position = { pos.x, pos.y, 0.0f };
	target->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
	target->TexCoords = { 0.0f, 0.0f };
	target->TexId = texId;
	target++;

	//Vertex v2; 
	target->Position = { pos.x + size.x, pos.y, 0.0f };
	target->Color = { 1.0f, 0.0f, 0.0f, 1.0f };
	target->TexCoords = { 1.0f, 0.0f };
	target->TexId = texId;
	target++;

	//Vertex v3;
	target->Position = { pos.x + size.x, pos.y + size.y, 0.0f };
	target->Color = { 0.0f, 1.0f, 0.0f, 1.0f };
	target->TexCoords = { 1.0f, 1.0f };
	target->TexId = texId;
	target++;

	//Vertex v4;
	target->Position = { pos.x, pos.y + size.y, 0.0f };
	target->Color = { 0.0f, 0.0f, 1.0f, 1.0f };
	target->TexCoords = { 0.0f, 1.0f };
	target->TexId = texId;
	target++;

	return target;
}


void VertexArray::Bind() const
{
	GlApiCall(glBindVertexArray(m_RendererId));
}

void VertexArray::Unbind() const
{
	GlApiCall(glBindVertexArray(0));
}
