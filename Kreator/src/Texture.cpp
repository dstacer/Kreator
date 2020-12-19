#include "stb_image/stb_image.h"

#include "Texture.h"

Texture::Texture(const std::string& filepath)
	: m_RendererId(0),
	  m_Filepath(filepath),
	  m_Buffer(nullptr),
	  m_Width(0),
	  m_Height(0),
	  m_Channels(0)
{
	// data is flipped for .png.  Must set appropriate for other
	// texture formats
	stbi_set_flip_vertically_on_load(true);
	m_Buffer = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_Channels, 4);

	GlApiCall(glGenTextures(1, &m_RendererId));
	Bind();

	GlApiCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GlApiCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GlApiCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP));
	GlApiCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP));

	GlApiCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Buffer));
	Unbind();

	if (m_Buffer)
		stbi_image_free(m_Buffer);
}

Texture::~Texture()
{
	GlApiCall(glDeleteTextures(1, &m_RendererId));
}

void Texture::Bind(unsigned int texSlot) const
{
	GlApiCall(glActiveTexture(GL_TEXTURE0 + texSlot));
	GlApiCall(glBindTexture(GL_TEXTURE_2D, m_RendererId));
}

void Texture::Unbind() const
{
	GlApiCall(glBindTexture(GL_TEXTURE_2D, 0));
}
