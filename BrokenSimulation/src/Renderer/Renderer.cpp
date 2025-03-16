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

	void Renderer::BeginScene()
	{
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray)
	{
		shader->Bind();
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
