#pragma once

#include <memory>

#include "VertexArray.h"
#include "Renderer/Shader.h"

namespace BrokenSim
{
	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(unsigned int width, unsigned int height);

		static void BeginScene();
		static void EndScene();

		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

		void Draw(const VertexArray& va, unsigned int count = 0) const;
	};
}
