#include "fpch.h"
#include "OpenGLTexture.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Feur {
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
	{
		F_PROFILE_FUNCTION();

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			F_PROFILE_SCOPE("stbi_load_OpenGLTexture2D::OpenGLTexture2D(const std::string& path)");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}
		F_CORE_ASSERT(data, "OpenGLTexture2D: Failed to load Texture2D");

		m_Width = width;
		m_Height = height;

		GLenum internalFormat, dataformat = 0;
		switch (channels) {
			case 3:	internalFormat = GL_RGB8;  dataformat = GL_RGB;  break;
			case 4:	internalFormat = GL_RGBA8; dataformat = GL_RGBA; break;
		}

		F_CORE_ASSERT(internalFormat & dataformat, "Texture format not supported!")

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataformat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		F_PROFILE_FUNCTION();

		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		F_PROFILE_FUNCTION();

		glBindTextureUnit(slot, m_RendererID);
	}
}