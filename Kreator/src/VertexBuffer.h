#pragma once

struct Vec4 {
	float x, y, z, w;
};

struct Vec3 {
	float x, y, z;
};

struct Vec2 {
	float x, y;
};

struct Vertex
{
	Vec3 Position;
	Vec4 Color;
	Vec2 TexCoords;
	float TexId;
};

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
	void Fill(const void* data, unsigned int size);
	
};
