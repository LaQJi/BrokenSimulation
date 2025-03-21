#include "bspch.h"
#include "Core/Log.h"
#include "Core/Macros.h"
#include "Renderer/VertexArray.h"
#include "Utils/Utils.h"

namespace BrokenSim
{
	static GLenum DataTypeToGLType(DataType type)
	{
		switch (type)
		{
			case DataType::Float:    return GL_FLOAT;
			case DataType::Float2:   return GL_FLOAT;
			case DataType::Float3:   return GL_FLOAT;
			case DataType::Float4:   return GL_FLOAT;
			case DataType::Mat3:     return GL_FLOAT;
			case DataType::Mat4:     return GL_FLOAT;
			case DataType::Int:      return GL_INT;
			case DataType::Int2:     return GL_INT;
			case DataType::Int3:     return GL_INT;
			case DataType::Int4:     return GL_INT;
			case DataType::Bool:     return GL_BOOL;
		}

		BS_CORE_ASSERT(false, "Unknown DataType!");
		return 0;
	}

	VertexArray::VertexArray()
	{
		GLCall(glGenVertexArrays(1, &rendererID));
	}

	VertexArray::~VertexArray()
	{
		GLCall(glDeleteVertexArrays(1, &rendererID));
	}

	void VertexArray::Bind() const
	{
		GLCall(glBindVertexArray(rendererID));
	}

	void VertexArray::Unbind() const
	{
		GLCall(glBindVertexArray(0));
	}

	void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		BS_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		Bind();
		vertexBuffer->Bind();
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			switch (element.type)
			{
				case DataType::Float:
				case DataType::Float2:
				case DataType::Float3:
				case DataType::Float4:
				{
					GLCall(glEnableVertexAttribArray(m_VertexBufferIndex));
					GLCall(glVertexAttribPointer(m_VertexBufferIndex,
						element.GetComponentCount(),
						DataTypeToGLType(element.type),
						element.normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)element.offset));
					m_VertexBufferIndex++;
					break;
				}
				case DataType::Mat3:
				case DataType::Mat4:
				{
					unsigned int count = element.GetComponentCount();
					for (unsigned int i = 0; i < count; i++)
					{
						GLCall(glEnableVertexAttribArray(m_VertexBufferIndex));
						GLCall(glVertexAttribPointer(m_VertexBufferIndex,
							count,
							DataTypeToGLType(element.type),
							element.normalized ? GL_TRUE : GL_FALSE,
							layout.GetStride(),
							(const void*)(element.offset + sizeof(float) * count * i)));
						GLCall(glVertexAttribDivisor(m_VertexBufferIndex, 1));
						m_VertexBufferIndex++;
					}
					break;
				}
				case DataType::Int:
				case DataType::Int2:
				case DataType::Int3:
				case DataType::Int4:
				case DataType::Bool:
				{
					GLCall(glEnableVertexAttribArray(m_VertexBufferIndex));
					GLCall(glVertexAttribIPointer(m_VertexBufferIndex,
						element.GetComponentCount(),
						DataTypeToGLType(element.type),
						layout.GetStride(),
						(const void*)element.offset));
					m_VertexBufferIndex++;
					break;
				}
				default:
					BS_CORE_ASSERT(false, "Unknown DataType!");
			}
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		Bind();
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

	const std::vector<std::shared_ptr<VertexBuffer>>& VertexArray::GetVertexBuffers() const
	{
		return m_VertexBuffers;
	}

	const std::shared_ptr<IndexBuffer>& VertexArray::GetIndexBuffer() const
	{
		return m_IndexBuffer;
	}
}
