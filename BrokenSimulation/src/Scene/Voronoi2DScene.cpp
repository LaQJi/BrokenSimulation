#include "bspch.h"
#include "Scene/Voronoi2DScene.h"

#include "Renderer/Renderer.h"
#include "Utils/Utils.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace BrokenSim
{
	Voronoi2DScene::Voronoi2DScene()
	{
		std::shared_ptr<Shader> shader = std::make_shared<Shader>("res/Shaders/voronoi2D_v.shader", "res/Shaders/voronoi2D_f.shader");

		Init(shader);

		// 创建一个和窗口大小相同的四边形
		float positions[] = {
			-1.0f, -1.0f,
			 1.0f, -1.0f,
			 1.0f,  1.0f,
			-1.0f,  1.0f
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		m_VertexArray = std::make_shared<VertexArray>();
		std::shared_ptr<VertexBuffer> vb = std::make_shared<VertexBuffer>(positions, 4 * 2 * sizeof(float));
		std::shared_ptr<IndexBuffer> ib = std::make_shared<IndexBuffer>(indices, 6);
		VertexBufferLayout layout({
			{ DataType::Float2, "a_Position" }
			});
		vb->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vb);
		m_VertexArray->SetIndexBuffer(ib);
	}

	Voronoi2DScene::~Voronoi2DScene()
	{
	}

	void Voronoi2DScene::OnUpdate(TimeStep ts, Camera& camera)
	{
		m_Shader->Bind();

		unsigned int index = 0;
		
		for (auto& object : m_Objects)
		{
			if (std::dynamic_pointer_cast<Point2DObject>(object.second))
			{
				std::shared_ptr<Point2DObject> point = std::dynamic_pointer_cast<Point2DObject>(object.second);
				glm::vec2 pos = point->GetPointPosition();
				glm::vec3 color = point->GetColor();

				m_Shader->SetUniform3f("u_Colors[" + std::to_string(index) + "]", color);
				m_Shader->SetUniform2f("u_Points[" + std::to_string(index) + "]", pos);
			}
			index++;
		}

		m_Shader->SetUniform1i("u_NumPoints", m_Objects.size());
	}

	void Voronoi2DScene::OnRender()
	{
		m_Shader->Bind();
		Renderer::DrawIndexed(m_VertexArray);
	}

	void Voronoi2DScene::OnImGuiRender()
	{
		ImGui::Text("Points2D");
		for (auto& object : m_Objects)
		{
			if (std::dynamic_pointer_cast<Point2DObject>(object.second))
			{
				std::shared_ptr<Point2DObject> point = std::dynamic_pointer_cast<Point2DObject>(object.second);
				ImGui::PushID(point->GetID());

				ImGui::Text("Point: %s", point->GetName().c_str());
				ImGui::ColorEdit3("Color", glm::value_ptr(point->GetColor()));
				ImGui::SliderFloat2("Position", glm::value_ptr(point->GetPointPosition()), 0.0f, 1.0f);

				ImGui::PopID();
			}
		}
	}

	void Voronoi2DScene::SetViewportSize(float width, float height)
	{
		m_Shader->Bind();
		m_Shader->SetUniform2f("u_ViewportSize", width, height);
	}

	std::shared_ptr<Object> Voronoi2DScene::CreateModelObject(const std::string& path, const std::string& name, Object* parent)
	{
		return nullptr;
	}

	std::shared_ptr<Object> Voronoi2DScene::CreateEmptyObject(const std::string& name, Object* parent)
	{
		return nullptr;
	}

	std::shared_ptr<Object> Voronoi2DScene::CreatePoint2D(const std::string& name)
	{
		unsigned int id = AssignID();
		std::shared_ptr<Object> point = std::make_shared<Point2DObject>(id, name);
		m_Objects[id] = point;
		return point;
	}
}
