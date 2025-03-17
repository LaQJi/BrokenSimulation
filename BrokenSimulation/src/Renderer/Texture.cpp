#include "bspch.h"
#include "Core/Macros.h"
#include "Renderer/Texture.h"
#include "Utils/Utils.h"

#include <stb_image/stb_image.h>

namespace BrokenSim
{
	static GLenum ImageFormatToGLInternalFormat(ImageFormat format)
	{
		switch (format)
		{
			case ImageFormat::RGB: return GL_RGB8;
			case ImageFormat::RGBA: return GL_RGBA8;
		}

		BS_CORE_ASSERT(false, "Unknown ImageFormat!");
		return 0;
	}

	static GLenum ImageFormatToGLDataFormat(ImageFormat format)
	{
		switch (format)
		{
			case ImageFormat::RGB: return GL_RGB;
			case ImageFormat::RGBA: return GL_RGBA;
		}

		BS_CORE_ASSERT(false, "Unknown ImageFormat!");
		return 0;
	}


	Texture::Texture(const std::string& path)
		: m_Path(path), m_Width(0), m_Height(0), m_RendererID(0), m_InternalFormat(0), m_DataFormat(0)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

		if (data)
		{
			m_Width = width;
			m_Height = height;

			ImageFormat format = ImageFormat::None;
			if (channels == 3)
			{
				format = ImageFormat::RGB;
			}
			else if (channels == 4)
			{
				format = ImageFormat::RGBA;
			}

			if (format == ImageFormat::None)
			{
				BS_CORE_ASSERT(false, "Format not supported!");
			}
			else
			{
				m_InternalFormat = ImageFormatToGLInternalFormat(format);
				m_DataFormat = ImageFormatToGLDataFormat(format);
			}

			GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID));
			GLCall(glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height));

			GLCall(glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			GLCall(glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

			GLCall(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT));
			GLCall(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT));

			GLCall(glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data));
			stbi_image_free(data);
		}
		else
		{
			stbi_image_free(data);
			BS_CORE_ASSERT(false, "Failed to load image!");
		}
	}

	Texture::Texture(const TextureSpecification& spec)
		: m_Spec(spec),
		m_Width(m_Spec.width), m_Height(m_Spec.height)
	{
		m_InternalFormat = ImageFormatToGLInternalFormat(m_Spec.format);
		m_DataFormat = ImageFormatToGLDataFormat(m_Spec.format);

		GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID));
		GLCall(glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height));

		GLCall(glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

		GLCall(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCall(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT));
	}

	Texture::~Texture()
	{
		GLCall(glDeleteTextures(1, &m_RendererID));
	}

	void Texture::SetData(void* data, unsigned int size)
	{
		// 获取每个像素所占用的有效字节数	
		unsigned int bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		BS_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
		GLCall(glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data));
	}

	void Texture::Bind(unsigned int slot) const
	{
		GLCall(glBindTextureUnit(slot, m_RendererID));
	}

	bool Texture::operator==(const Texture& other) const
	{
		return false;
	}

}
