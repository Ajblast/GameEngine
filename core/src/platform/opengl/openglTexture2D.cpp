#include "gravpch.h"
#include "openglTexture2D.h"

#include <stb_image.h>

GRAVEngine::Rendering::openglTexture2D::openglTexture2D(uint32 width, uint32 height) : texture2D(width, height)
{
	m_InternalFormat = GL_RGBA8;
	m_DataFormat = GL_RGBA;

	glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
	glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

	glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

GRAVEngine::Rendering::openglTexture2D::openglTexture2D(const std::string& path) : texture2D(path)
{
	int width, height, channels;
	stbi_set_flip_vertically_on_load(1);
	stbi_uc* data = nullptr;
	{
		//HZ_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
		data = stbi_load(path.c_str(), &width, &height, &channels, 0);
	}
	// Check if the data was actually loadded
	GRAV_ASSERT(data); //"Failed to load image!"
	m_Width = width;
	m_Height = height;

	GLenum internalFormat = 0, dataFormat = 0;
	if (channels == 4)
	{
		internalFormat = GL_RGBA8;
		dataFormat = GL_RGBA;
	}
	else if (channels == 3)
	{
		internalFormat = GL_RGB8;
		dataFormat = GL_RGB;
	}

	m_InternalFormat = internalFormat;
	m_DataFormat = dataFormat;

	// Check if the format is supported
	GRAV_ASSERT(internalFormat & dataFormat); // "Format not supported!"

	// Create the texture
	glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
	glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

	// Set the texture parameters
	glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

	// Free the image from stbi
	stbi_image_free(data);
}

GRAVEngine::Rendering::openglTexture2D::~openglTexture2D()
{
	// Delete the texture from the graphics card
	glDeleteTextures(1, &m_RendererID);
}

void GRAVEngine::Rendering::openglTexture2D::setData(void* data, uint32 size)
{
	// Set the data of the texture
	uint32 bpp = m_DataFormat == GL_RGBA ? 4 : 3;
	GRAV_ASSERT(size == m_Width * m_Height * bpp); //"Data must be entire texture!"
	glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
}

void GRAVEngine::Rendering::openglTexture2D::bind(uint32 slot) const
{
	// Bind this texture to the slot
	glBindTextureUnit(slot, m_RendererID);
}
