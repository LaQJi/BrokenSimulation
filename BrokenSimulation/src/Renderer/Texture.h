#pragma once

#include <string>

#include <GL/glew.h>

namespace BrokenSim
{
	enum class ImageFormat
	{
		None = 0,
		RGB = 1,
		RGBA = 2
	};

	struct TextureSpecification
	{
		unsigned int width = 1;
		unsigned int height = 1;
		ImageFormat format = ImageFormat::RGBA;
	};

	class Texture
	{
	public:
		Texture(const std::string& path);
		Texture(const TextureSpecification& spec);
		~Texture();

		void SetData(void* data, unsigned int size);

		void Bind(unsigned int slot = 0) const;

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }
		inline unsigned int GetRendererID() const { return m_RendererID; }

		inline const std::string& GetPath() const { return m_Path; }

		bool operator==(const Texture& other) const;

	private:
		TextureSpecification m_Spec;

		std::string m_Path;
		unsigned int m_Width, m_Height;
		unsigned int m_RendererID;

		GLenum m_InternalFormat;
		GLenum m_DataFormat;
	};
}
