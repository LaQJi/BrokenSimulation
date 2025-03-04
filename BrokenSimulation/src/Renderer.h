#pragma once

#include <string>
#include <fstream>
#include <sstream>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "util.h"

class Renderer
{
public:
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void DrawPoints(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

	// ������ɫ����������Ȼ�����
	void Clear() const;
};
