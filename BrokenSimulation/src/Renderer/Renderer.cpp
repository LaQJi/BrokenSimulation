#include "bspch.h"
#include "Renderer/Renderer.h"

#include "Utils/Utils.h"

namespace BrokenSim
{
	std::unique_ptr<Renderer::SceneData> Renderer::s_SceneData = std::make_unique<Renderer::SceneData>();

	void Renderer::Init()
	{
		// 启用混合
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// 启用深度测试和背面剔除
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
	}

	void Renderer::Shutdown()
	{
	}

	void Renderer::OnWindowResize(unsigned int width, unsigned int height)
	{
		glViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		s_SceneData->viewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::SetViewportSize(unsigned int width, unsigned int height)
	{
		glViewport(0, 0, width, height);
	}

	void Renderer::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void Renderer::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetUniformMat4f("u_ViewProjection", s_SceneData->viewProjectionMatrix);
		shader->SetUniformMat4f("u_Transform", transform);

		vertexArray->Bind();
		vertexArray->GetIndexBuffer()->Bind();
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void Renderer::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, unsigned int count)
	{
		vertexArray->Bind();
		vertexArray->GetIndexBuffer()->Bind();
		unsigned int indexCount = count == 0 ? vertexArray->GetIndexBuffer()->GetCount() : count;
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
	}
}
