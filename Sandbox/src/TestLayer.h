#pragma once

#include "BrokenSimulation.h"

#include "Scene/Scene3D.h"
#include "Scene/Voronoi2DScene.h"


#include <glew/include/GL/glew.h>
#include <GLFW/include/GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace BrokenSim
{
	class TestLayer : public Layer
	{
	public:
		TestLayer()
			: Layer("TestLayer"),
			m_ViewportSize(1280.0f, 720.0f)
		{
			FrameBufferSpecification spec;
			spec.width = 1280;
			spec.height = 720;
			spec.attachments = { {TextureFormat::RGBA8}, TextureFormat::DEPTH24STENCIL8 };
			m_FrameBuffer = std::make_shared<FrameBuffer>(spec);

			FrameBufferSpecification voronoiSpec;
			voronoiSpec.width = 1280;
			voronoiSpec.height = 720;
			voronoiSpec.attachments = { {TextureFormat::RGBA8} };
			m_VoronoiFrameBuffer = std::make_shared<FrameBuffer>(voronoiSpec);

			float vertices[3 * 3] = {
				-0.5f, -0.5f, 0.0f,
				 0.5f, -0.5f, 0.0f,
				 0.0f,  0.5f, 0.0f
			};

			unsigned int indices[3] = { 0, 1, 2 };

			m_VertexArray = std::make_shared<VertexArray>();
			std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(vertices, sizeof(vertices));
			std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(indices, sizeof(indices) / sizeof(unsigned int));

			VertexBufferLayout layout({
				{ DataType::Float3, "a_Position" }
				});

			vertexBuffer->SetLayout(layout);

			m_VertexArray->AddVertexBuffer(vertexBuffer);
			m_VertexArray->SetIndexBuffer(indexBuffer);

			m_Shader = std::make_shared<Shader>("res/Shaders/test_v.shader", "res/Shaders/test_f.shader");
			m_Shader->Bind();

			m_ActiveScene = std::make_shared<Scene3D>();
			m_Voronoi2DScene = std::make_shared<Voronoi2DScene>();
			//m_ActiveScene->CreateModelObject(Object::Type::Model, "res/Models/ring.obj", "Model", nullptr);

			for (int i = 0; i < 8; i++)
			{
				std::stringstream ss;
				ss << "res/Models/ring_" << i << ".obj";
				m_ActiveScene->CreateModelObject(ss.str(), "Model" + std::to_string(i), nullptr);

				m_Voronoi2DScene->CreatePoint2D("Point2D" + std::to_string(i));
			}
			m_Voronoi2DScene->SetViewportSize(1280, 720);
		}

		void OnUpdate(TimeStep ts)
		{
			//Renderer::BeginScene(m_CameraController.GetCamera());
			Renderer::BeginScene(m_Camera);

			//m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
			m_Camera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);

			if (m_Focused)
			{
				//m_CameraController.OnUpdate(ts);
				m_Camera.OnUpdate(ts);
			}

			Renderer::OnWindowResize((unsigned int)m_ViewportSize.x, (unsigned int)m_ViewportSize.y);

			m_FrameBuffer->Bind();

			glClearColor(0.25f, 0.25f, 0.25f, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//m_Shader->Bind();
			//m_Shader->SetUniform4f("v_Color", m_Color);
			//Renderer::Submit(m_Shader, m_VertexArray);

			m_ActiveScene->OnUpdate(ts, m_Camera);
			m_ActiveScene->OnRender();

			m_FrameBuffer->Unbind();

			m_VoronoiFrameBuffer->Bind();

			//glClear(GL_COLOR_BUFFER_BIT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


			m_Voronoi2DScene->OnUpdate(ts, m_Camera);

			glm::vec2 size = m_VoronoiFrameBuffer->GetSize();
			m_Voronoi2DScene->SetViewportSize(size.x, size.y);

			m_Voronoi2DScene->OnRender();

			m_VoronoiFrameBuffer->Unbind();
		}

		void OnEvent(BrokenSim::Event& event) override
		{
			BS_CORE_DEBUG(event);

			//m_CameraController.OnEvent(event);
			m_Camera.OnEvent(event);
		}

		void OnImGuiRender() override
		{
			static bool dockspaceOpen = true;
			static bool opt_fullscreen = true;
			static bool opt_padding = false;
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

			if (opt_fullscreen)
			{
				const ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(viewport->WorkPos);
				ImGui::SetNextWindowSize(viewport->WorkSize);
				ImGui::SetNextWindowViewport(viewport->ID);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
				window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			}

			if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
				window_flags |= ImGuiWindowFlags_NoBackground;

			if (!opt_padding)
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

			ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);

			if (!opt_padding)
				ImGui::PopStyleVar();

			if (opt_fullscreen)
				ImGui::PopStyleVar(2);

			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("New Scene", "Ctrl+N")) {}
					if (ImGui::MenuItem("Open Scene", "Ctrl+O")) {}
					if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {}
					if (ImGui::MenuItem("Exit")) {}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Edit"))
				{
					if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
					if (ImGui::MenuItem("Redo", "Ctrl+Y", false, false)) {}
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}
			ImGui::End();


			ImGui::ShowDemoWindow();


			ImGui::Begin("Settings");

			m_ActiveScene->OnImGuiRender();

			ImGui::End();


			ImGui::Begin("FrameBuffer");

			m_Focused = ImGui::IsWindowFocused();
			m_Hovered = ImGui::IsWindowHovered();

			Application::Get().GetImGuiLayer().SetBlockEvents(!m_Hovered);

			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
			{
				m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
				m_FrameBuffer->Resize((unsigned int)m_ViewportSize.x, (unsigned int)m_ViewportSize.y);
				m_ViewportSize = { m_FrameBuffer->GetSpecification().width, m_FrameBuffer->GetSpecification().height };
			}
			unsigned int textureID = m_FrameBuffer->GetColorAttachmentRendererID();
			ImGui::Image((ImTextureID)(uintptr_t)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));
			ImGui::End();

			ImGui::Begin("VoronoiFrameBuffer");

			ImVec2 viewportSize = ImGui::GetContentRegionAvail();
			//BS_CORE_INFO("Viewport Size: {0}, {1}", viewportSize.x, viewportSize.y);
			m_Voronoi2DScene->SetViewportSize(viewportSize.x, viewportSize.y);

			unsigned int voronoiTextureID = m_VoronoiFrameBuffer->GetColorAttachmentRendererID();
			float minXY = std::min(viewportSize.x, viewportSize.y);

			float offsetX = (viewportSize.x - minXY) / 2.0f;
			float offsetY = (viewportSize.y - minXY) / 2.0f;

			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offsetY);

			ImGui::Image((ImTextureID)(uintptr_t)voronoiTextureID, ImVec2{ minXY, minXY });

			ImGui::End();

			ImGui::Begin("Points Setting");

			m_Voronoi2DScene->OnImGuiRender();

			ImGui::End();
		}
	private:
		std::shared_ptr<VertexArray> m_VertexArray;
		std::shared_ptr<Shader> m_Shader;
		glm::vec4 m_Color = { 0.2f, 0.3f, 0.8f, 1.0f };

		std::shared_ptr<FrameBuffer> m_FrameBuffer;
		std::shared_ptr<FrameBuffer> m_VoronoiFrameBuffer;

		//OrthographicCameraController m_CameraController;
		Camera m_Camera;

		std::shared_ptr<Scene3D> m_ActiveScene;
		std::shared_ptr<Voronoi2DScene> m_Voronoi2DScene;

		glm::vec2  m_ViewportSize = { 0.0f, 0.0f };

		bool m_Hovered = false;
		bool m_Focused = false;
	};
}
