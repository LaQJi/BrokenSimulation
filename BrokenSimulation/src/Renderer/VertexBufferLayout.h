#pragma once

namespace BrokenSim
{
	enum class DataType
	{
		None = 0,
		Float, Float2, Float3, Float4,
		Mat3, Mat4,
		Int, Int2, Int3, Int4,
		Bool
	};

	static unsigned int DataTypeSize(DataType type);

	struct VertexBufferElement
	{
		std::string name;
		DataType type;
		unsigned int size;
		size_t offset;
		bool normalized;
		
		VertexBufferElement() = default;
		VertexBufferElement(DataType type, const std::string& name, bool normalized = false);

		unsigned int GetComponentCount() const;
	};

	class VertexBufferLayout
	{
	public:
		VertexBufferLayout();
		VertexBufferLayout(const std::initializer_list<VertexBufferElement>& elements);

		const unsigned int GetStride() const { return m_Stride; }
		const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }

		std::vector<VertexBufferElement>::iterator begin();
		std::vector<VertexBufferElement>::iterator end();
		std::vector<VertexBufferElement>::const_iterator begin() const;
		std::vector<VertexBufferElement>::const_iterator end() const;

	private:
		std::vector<VertexBufferElement> m_Elements;
		unsigned int m_Stride = 0;

		void CalculateOffsetsAndStride();
	};
}
