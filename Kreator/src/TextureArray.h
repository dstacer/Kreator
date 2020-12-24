#pragma once

#include <string>
#include <vector>

#include "Renderer.h"

class TextureArray
{
private:
	unsigned int m_RendererId;
	std::vector<std::string> m_Filepaths;
	unsigned char* m_Buffers[16];
	int m_Width, m_Height, m_Channels;
	int m_Layers;

public:
	TextureArray(const std::vector<std::string> filepaths);
	~TextureArray();

	void Bind(unsigned int texSlot = 0) const;
	void Unbind() const;

	int GetWidth() const { return m_Width; }
	int GetHeight() const { return m_Height; }
};
