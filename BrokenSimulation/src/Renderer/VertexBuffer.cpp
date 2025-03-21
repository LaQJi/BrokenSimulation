#include "bspch.h"
#include "Renderer/VertexBuffer.h"
#include "Utils/Utils.h"

namespace BrokenSim
{
	VertexBuffer::VertexBuffer(unsigned int size)
	{
		GLCall(glGenBuffers(1, &rendererID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
	}

	VertexBuffer::VertexBuffer(const float* data, unsigned int size)
	{
		GLCall(glGenBuffers(1, &rendererID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	}

	VertexBuffer::~VertexBuffer()
	{
		GLCall(glDeleteBuffers(1, &rendererID));
	}

	void VertexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
	}

	void VertexBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	void VertexBuffer::SetData(const void* data, unsigned int size)
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
	}

	void VertexBuffer::SetLayout(const VertexBufferLayout& layout)
	{
		m_Layout = layout;
	}

	const VertexBufferLayout& VertexBuffer::GetLayout() const
	{
		return m_Layout;
	}
}
