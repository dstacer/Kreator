#pragma once

#include <string>

#include "Renderer.h"

class Texture
{
private:
	unsigned int m_RendererId;
	std::string m_Filepath;
	unsigned char* m_Buffer;
	int m_Width, m_Height, m_Channels;

public:
	Texture(uint32_t width, uint32_t height, void* data);
	Texture(const std::string& filepath);
	
	~Texture();

	void Bind(unsigned int texSlot = 0) const;
	void BindUnit(unsigned int texSlot) const;
	void Unbind() const;

	int GetWidth() const { return m_Width; }
	int GetHeight() const { return m_Height; }
	void SetData(void* data, uint32_t size);
};
