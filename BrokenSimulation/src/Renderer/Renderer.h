#pragma once

#include <memory>

#include "VertexArray.h"
#include "Renderer/OrthographicCamera.h"
#include "Scene/Camera.h"
#include "Renderer/Shader.h"

namespace BrokenSim
{
	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(unsigned int width, unsigned int height);

		static void BeginScene(OrthographicCamera& camera);
		static void BeginScene(Camera& camera);
		static void EndScene();

		void SetViewportSize(unsigned int width, unsigned int height);

		void SetClearColor(const glm::vec4& color);
		void Clear();

		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, unsigned int count = 0);

	private:
		struct SceneData
		{
			glm::mat4 viewProjectionMatrix;
		};

		static std::unique_ptr<SceneData> s_SceneData;
	};
}
