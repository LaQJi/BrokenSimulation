#include "Renderer/Renderer.h"

namespace BrokenSim
{
	void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
	{
		shader.Bind();
		va.Bind();
		ib.Bind();

		GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
	}

	void Renderer::DrawPoints(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
	{
		shader.Bind();
		va.Bind();
		ib.Bind();

		GLCall(glDrawElements(GL_POINTS, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
	}

	void Renderer::DrawModel(const Model& model, const Shader& shader) const
	{
		shader.Bind();
		model.Bind();

		GLCall(glDrawElements(GL_TRIANGLES, model.GetIndexCount(), GL_UNSIGNED_INT, nullptr));
	}

	void Renderer::Clear() const
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}
}
