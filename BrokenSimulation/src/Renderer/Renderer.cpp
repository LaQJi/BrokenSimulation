#include "bspch.h"
#include "Renderer/Renderer.h"

#include "Utils/Utils.h"

namespace BrokenSim
{
	void Renderer::Init()
	{
		// 启用混合
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		// 启用深度测试和背面剔除
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glEnable(GL_CULL_FACE));
	}

	void Renderer::Shutdown()
	{
	}

	void Renderer::OnWindowResize(unsigned int width, unsigned int height)
	{
		GLCall(glViewport(0, 0, width, height));
	}

	void Renderer::BeginScene(OrthographicCamera camera)
	{
		s_SceneData->viewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::SetViewportSize(unsigned int width, unsigned int height)
	{
		GLCall(glViewport(0, 0, width, height));
	}

	void Renderer::SetClearColor(const glm::vec4& color)
	{
		GLCall(glClearColor(color.r, color.g, color.b, color.a));
	}

	void Renderer::Clear()
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetUniformMat4f("u_ViewProjection", s_SceneData->viewProjectionMatrix);
		shader->SetUniformMat4f("u_Transform", transform);

		vertexArray->Bind();
		GLCall(glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr));
	}

	void Renderer::Draw(const VertexArray& va, unsigned int count) const
	{
		va.Bind();
		unsigned int indexCount = count == 0 ? va.GetIndexBuffer()->GetCount() : count;
		GLCall(glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr));
	}
}
