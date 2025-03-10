#pragma once

#include "Renderer/VertexBuffer.h"
#include "Renderer/VertexBufferLayout.h"
#include "Utils/util.h"

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;

private:
	unsigned int rendererID;
};
