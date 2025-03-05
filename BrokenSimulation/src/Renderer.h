#pragma once

#include <string>
#include <fstream>
#include <sstream>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Model.h"
#include "util.h"

class Renderer
{
public:
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void DrawPoints(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void DrawModel(const Model& model, const Shader& shader) const;

	// 清理颜色缓冲区和深度缓冲区
	void Clear() const;
};
