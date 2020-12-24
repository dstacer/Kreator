#include "stb_image/stb_image.h"

#include "TextureArray.h"

TextureArray::TextureArray(const std::vector<std::string> filepaths)
	: m_RendererId(0),
	m_Filepaths(filepaths),
	m_Width(0),
	m_Height(0),
	m_Channels(0),
	m_Layers(filepaths.size()+1)
{
	// data is flipped for .png.  Must set appropriate for other
	// texture formats
	stbi_set_flip_vertically_on_load(true);
	for (int i = 0; i < m_Filepaths.size(); ++i)
	{
		m_Buffers[i+1] = stbi_load(m_Filepaths[i].c_str(), &m_Width, &m_Height, &m_Channels, 4);
	}

	// Set the first texture buffer to white.
	size_t bufferSize = (size_t)(m_Width * m_Height*4);
	m_Buffers[0] = (unsigned char*)malloc(bufferSize);
	memset(m_Buffers[0], 255, bufferSize);


	GlApiCall(glGenTextures(1, &m_RendererId));

	Bind();
	// Allocate the storage.
	GlApiCall(glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, m_Width, m_Height, m_Layers,
							0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));

	GlApiCall(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GlApiCall(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GlApiCall(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GlApiCall(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT));

	// The following link helps clarify the uploading of the texture data in this loop:
    // https://stackoverflow.com/questions/52339703/problems-using-gltexsubimage3d-correctly
	for (int i = 0; i < m_Layers; ++i)
	{
		GlApiCall(glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, m_Width, m_Height,
			1, GL_RGBA, GL_UNSIGNED_BYTE, m_Buffers[i]));
		
		if (m_Buffers[i])
			stbi_image_free(m_Buffers[i]);
	}
	Unbind();

}

TextureArray::~TextureArray()
{
	GlApiCall(glDeleteTextures(1, &m_RendererId));
}

void TextureArray::Bind(unsigned int texSlot) const
{
	GlApiCall(glActiveTexture(GL_TEXTURE0 + texSlot));
	GlApiCall(glBindTexture(GL_TEXTURE_2D_ARRAY, m_RendererId));
}

void TextureArray::Unbind() const
{
	GlApiCall(glBindTexture(GL_TEXTURE_2D_ARRAY, 0));
}
