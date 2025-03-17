#pragma once

namespace BrokenSim
{
	enum class TextureFormat
	{
		None = 0,
		RGBA8,
		RGBA16F,
		RGBA32F,
		DEPTH24STENCIL8,
		RED_INTEGER
	};

	struct TextureSpecification
	{
		TextureSpecification() = default;
		TextureSpecification(TextureFormat format)
			: textureFormat(format)
		{
		}

		TextureFormat textureFormat = TextureFormat::None;
	};

	struct FrameBufferSpecification
	{
		unsigned int width = 0;
		unsigned int height = 0;
		unsigned int samples = 1;
		bool swapChainTarget = false;

		std::vector<TextureSpecification> attachments;
	};

	class FrameBuffer
	{
	public:
		FrameBuffer(const FrameBufferSpecification& spec);
		~FrameBuffer();

		void Invalidate();

		void Bind();
		void Unbind();

		void Resize(unsigned int width, unsigned int height);
		int ReadPixel(unsigned int attachmentIndex, int x, int y);

		void ClearAttachment(unsigned int attachmentIndex, int value);

		unsigned int GetColorAttachmentRendererID(unsigned int index = 0);

		const FrameBufferSpecification& GetSpecification() const { return m_Specification; }

	private:
		unsigned int m_RendererID = 0;
		FrameBufferSpecification m_Specification;

		std::vector<TextureSpecification> m_ColorAttachmentSpecs;
		TextureSpecification m_DepthAttachmentSpec = TextureFormat::None;

		std::vector<unsigned int> m_ColorAttachments;
		unsigned int m_DepthAttachment = 0;
	};
}
