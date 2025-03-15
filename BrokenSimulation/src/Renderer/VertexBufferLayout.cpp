#include "bspch.h"
#include "Core/Log.h"
#include "Renderer/VertexBufferLayout.h"
#include "Utils/Utils.h"

namespace BrokenSim
{
	unsigned int DataTypeSize(DataType type)
	{
		switch (type)
		{
			case DataType::Float:		 return 4;
			case DataType::Float2:		 return 4 * 2;
			case DataType::Float3:		 return 4 * 3;
			case DataType::Float4:		 return 4 * 4;
			case DataType::Mat3:		 return 4 * 3 * 3;
			case DataType::Mat4:		 return 4 * 4 * 4;
			case DataType::Int:			 return 4;
			case DataType::Int2:		 return 4 * 2;
			case DataType::Int3:		 return 4 * 3;
			case DataType::Int4:		 return 4 * 4;
			case DataType::Bool:		 return 1;
		}

		BS_CORE_DEBUG("Unknown DataType!");
		return 0;
	}

	unsigned int VertexBufferElement::GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT: return 4;
			case GL_UNSIGNED_INT: return 4;
			case GL_UNSIGNED_BYTE: return 1;
		}

		BS_CORE_ERROR("Unknown type!");
		return 0;
	}

	VertexBufferElement::VertexBufferElement(DataType type, const std::string& name, bool normalized)
		: type(type), name(name), size(DataTypeSize(type)), offset(0), normalized(normalized)
	{
	}

	unsigned int VertexBufferElement::GetComponentCount() const
	{
		switch (type)
		{
			case DataType::Float:		 return 1;
			case DataType::Float2:		 return 2;
			case DataType::Float3:		 return 3;
			case DataType::Float4:		 return 4;
			case DataType::Mat3:		 return 3 * 3;
			case DataType::Mat4:		 return 4 * 4;
			case DataType::Int:			 return 1;
			case DataType::Int2:		 return 2;
			case DataType::Int3:		 return 3;
			case DataType::Int4:		 return 4;
			case DataType::Bool:		 return 1;
		}

		BS_CORE_ERROR("Unknown DataType!");
		return 0;
	}

	VertexBufferLayout::VertexBufferLayout()
	{
	}

	VertexBufferLayout::VertexBufferLayout(const std::initializer_list<VertexBufferElement>& elements)
		: m_Elements(elements)
	{
		CalculateOffsetsAndStride();
	}

	std::vector<VertexBufferElement>::iterator VertexBufferLayout::begin()
	{
		return m_Elements.begin();
	}

	std::vector<VertexBufferElement>::iterator VertexBufferLayout::end()
	{
		return m_Elements.end();
	}

	std::vector<VertexBufferElement>::const_iterator VertexBufferLayout::begin() const
	{
		return m_Elements.begin();
	}

	std::vector<VertexBufferElement>::const_iterator VertexBufferLayout::end() const
	{
		return m_Elements.end();
	}

	void VertexBufferLayout::CalculateOffsetsAndStride()
	{
		unsigned int offset = 0;
		m_Stride = 0;
		for (auto& element : m_Elements)
		{
			element.offset = offset;
			offset += element.size;
			m_Stride += element.size;
		}
	}
}
