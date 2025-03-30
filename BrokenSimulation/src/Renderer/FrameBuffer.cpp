#include "bspch.h"
#include "Core/Macros.h"
#include "Renderer/FrameBuffer.h"
#include "Utils/Utils.h"

namespace BrokenSim
{
	constexpr auto MAX_FRAMEBUFFER_SIZE = 8192;

	static bool IsDepthFormat(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::DEPTH24STENCIL8:
			return true;
		}
		return false;
	}

	FrameBuffer::FrameBuffer(const FrameBufferSpecification& spec)
		: m_Specification(spec)
	{
		for (auto spec : m_Specification.attachments)
		{
			if (!IsDepthFormat(spec.textureFormat))
				m_ColorAttachmentSpecs.emplace_back(spec);
			else
				m_DepthAttachmentSpec = spec;
		}

		Invalidate();
	}

	FrameBuffer::~FrameBuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);
	}

	void FrameBuffer::Invalidate()
	{
		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
			glDeleteTextures(1, &m_DepthAttachment);

			m_ColorAttachments.clear();
			m_DepthAttachment = 0;
		}

		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		bool multisample = m_Specification.samples > 1;
		GLenum target = multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

		// 附加颜色缓冲区
		if (m_ColorAttachmentSpecs.size())
		{
			m_ColorAttachments.resize(m_ColorAttachmentSpecs.size());
			glCreateTextures(target, m_ColorAttachments.size(), m_ColorAttachments.data());

			for (size_t i = 0; i < m_ColorAttachments.size(); i++)
			{
				glBindTexture(target, m_ColorAttachments[i]);
				switch (m_ColorAttachmentSpecs[i].textureFormat)
				{
				case TextureFormat::RGBA8:
				{
					if (multisample)
					{
						glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_Specification.samples, GL_RGBA8, m_Specification.width, m_Specification.height, GL_FALSE);
					}
					else
					{
						glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.width, m_Specification.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					}
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, target, m_ColorAttachments[i], 0);
					break;
				}
				case TextureFormat::RED_INTEGER:
				{
					if (multisample)
					{
						glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_Specification.samples, GL_R32I, m_Specification.width, m_Specification.height, GL_FALSE);
					}
					else
					{
						glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, m_Specification.width, m_Specification.height, 0, GL_RED_INTEGER, GL_INT, nullptr);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					}
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, target, m_ColorAttachments[i], 0);
					break;
				}
				default:
					BS_CORE_ASSERT(false, "Unknown TextureFormat!");
				}
			}
		}

		// 附加深度缓冲区
		if (m_DepthAttachmentSpec.textureFormat != TextureFormat::None)
		{
			glCreateTextures(target, 1, &m_DepthAttachment);
			glBindTexture(target, m_DepthAttachment);

			switch (m_DepthAttachmentSpec.textureFormat)
			{
			case TextureFormat::DEPTH24STENCIL8:
			{
				if (multisample)
				{
					glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_Specification.samples, GL_DEPTH24_STENCIL8, m_Specification.width, m_Specification.height, GL_FALSE);
				}
				else
				{
					glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Specification.width, m_Specification.height);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				}
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, target, m_DepthAttachment, 0);
			}
			}
		}

		if (m_ColorAttachments.size() > 1)
		{
			BS_CORE_ASSERT(m_ColorAttachments.size() <= 4, "FrameBuffer only supports 4 attachments!");
			GLenum bufs[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_ColorAttachments.size(), bufs);
		}
		else if (m_ColorAttachments.empty())
		{
			glDrawBuffer(GL_NONE);
		}

		BS_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "FrameBuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specification.width, m_Specification.height);
	}

	void FrameBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::Resize(unsigned int width, unsigned int height)
	{
		if (width == 0 || height == 0 || width > MAX_FRAMEBUFFER_SIZE || height > MAX_FRAMEBUFFER_SIZE)
		{
			BS_CORE_WARN("Attempted to resize framebuffer to {0}, {1}", width, height);
			return;
		}

		m_Specification.width = width;
		m_Specification.height = height;

		Invalidate();
	}

	int FrameBuffer::ReadPixel(unsigned int attachmentIndex, int x, int y)
	{
		BS_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "Attachment index out of range!");

		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
	}

	void FrameBuffer::ClearAttachment(unsigned int attachmentIndex, int value)
	{
		BS_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "Attachment index out of range!");

		auto& spec = m_ColorAttachmentSpecs[attachmentIndex];
		GLenum format = 0;
		switch (spec.textureFormat)
		{
		case TextureFormat::RGBA8:
			format = GL_RGBA;
			break;
		case TextureFormat::RED_INTEGER:
			format = GL_RED_INTEGER;
			break;
		}
		glClearTexImage(m_ColorAttachments[attachmentIndex], 0, format, GL_INT, &value);
	}

	unsigned int FrameBuffer::GetColorAttachmentRendererID(unsigned int index)
	{
		BS_CORE_ASSERT(index < m_ColorAttachments.size(), "Index out of range!");
		return m_ColorAttachments[index];
	}
}
