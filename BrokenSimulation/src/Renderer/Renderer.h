#pragma once

#include "Renderer/VertexArray.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Shader.h"
#include "Renderer/Model.h"
#include "Utils/Utils.h"

namespace BrokenSim
{
	class Renderer
	{
	public:
		void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
		void DrawPoints(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
		void DrawModel(const Model& model, const Shader& shader) const;

		// 清理颜色缓冲区和深度缓冲区
		void Clear() const;
	};
}
