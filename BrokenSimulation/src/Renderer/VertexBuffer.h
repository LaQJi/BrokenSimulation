#pragma once

#include "Renderer/VertexBufferLayout.h"

namespace BrokenSim
{
	class VertexBuffer
	{
	public:
		VertexBuffer(unsigned int size);
		VertexBuffer(const void* data, unsigned int size);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;

		void SetData(const void* data, unsigned int size);

		void SetLayout(const VertexBufferLayout& layout);

		const VertexBufferLayout& GetLayout() const;

	private:
		unsigned int rendererID;
		VertexBufferLayout m_Layout;
	};
}