#pragma once

#include <vector>

#include "Utils/util.h"

namespace BrokenSim
{
	struct VertexBufferElement
	{
		unsigned int type;
		unsigned int count;
		unsigned char normalized;

		static unsigned int GetSizeOfType(unsigned int type);
	};

	class VertexBufferLayout
	{
	public:
		VertexBufferLayout();
		~VertexBufferLayout();

		template<typename T>
		void Push(unsigned int count);

		template<>
		void Push<float>(unsigned int count);

		template<>
		void Push<unsigned int>(unsigned int count);

		template<>
		void Push<unsigned char>(unsigned int count);

		inline const std::vector<VertexBufferElement> GetElements() const { return elements; }
		inline unsigned int GetStride() const { return stride; }

	private:
		std::vector<VertexBufferElement> elements;
		unsigned int stride;
	};
}
