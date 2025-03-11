#pragma once

#include "Utils/util.h"

namespace BrokenSim
{
	class IndexBuffer
	{
	public:
		IndexBuffer(const unsigned int* data, unsigned int count);
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;

		inline unsigned int GetCount() const { return count; }

	private:
		unsigned int rendererID;
		unsigned int count;
	};
}
