#include "EditorLayer.h"

#include <glew/include/GL/glew.h>
#include <GLFW/include/GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace BrokenSim
{
	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
	{
		m_FontPath = "res/Fonts/msyh.ttc";
		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF(m_FontPath.c_str(), 18.0f);
		
		m_WorkingDirectory = std::filesystem::current_path().string();

		m_EngineMode = EngineMode::EditMode;
	}

	void EditorLayer::OnAttach()
	{
		m_SceneViewportSize = { 1280.0f, 720.0f };
		m_VoronoiViewportSize = { 1280.0f, 720.0f };

		FrameBufferSpecification sceneSpec;
		sceneSpec.width = 720;
		sceneSpec.height = 720;
		sceneSpec.attachments = { {TextureFormat::RGBA8}, TextureFormat::DEPTH24STENCIL8 };
		m_SceneFrameBuffer = std::make_shared<FrameBuffer>(sceneSpec);

		FrameBufferSpecification voronoiSpec;
		voronoiSpec.width = 720;
		voronoiSpec.height = 720;
		voronoiSpec.attachments = { {TextureFormat::RGBA8} };
		m_VoronoiFrameBuffer = std::make_shared<FrameBuffer>(voronoiSpec);

		m_Scene = std::make_shared<Scene>();

		Application::Get().GetRenderSystem()->SetCurrentShader("BlinnPhong");

		for (int i = 0; i < 8; i++)
		{
			std::stringstream ss;
			ss << "res/Models/ring_" << i << ".obj";
			Entity* entity = m_Scene->CreateEntity();
			entity->AddComponent<ModelComponent>(ss.str());
		}
		
		m_SceneHierarchyPanel = SceneHierarchyPanel(m_Scene);
	}

	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnUpdate(TimeStep ts)
	{
		m_Scene->GetCamera()->SetViewportSize(m_SceneViewportSize.x, m_SceneViewportSize.y);

		if (m_SceneFocused)
		{
			m_Scene->GetCamera()->OnUpdate(ts);
		}
		
		Application::Get().GetRenderSystem()->OnWindowResize((unsigned int)m_SceneViewportSize.x, (unsigned int)m_SceneViewportSize.y);

		m_SceneFrameBuffer->Bind();

		Application::Get().GetRenderSystem()->SetClearColor({ 0.24f, 0.24f, 0.24f, 1.0f });
		Application::Get().GetRenderSystem()->Clear();
		
		Application::Get().GetRenderSystem()->OnUpdate(ts, m_Scene.get());

		m_SceneFrameBuffer->Unbind();

		if (m_SceneHierarchyPanel.GetSelectionContext())
		{
			m_SelectedEntity = m_SceneHierarchyPanel.GetSelectionContext();
		}

		// 若有选中的实体，则根据实体是否有VoronoiComponent切换场景状态
		if (m_SelectedEntity)
		{
			if (m_SelectedEntity->HasComponent<VoronoiComponent>())
			{
				// 若选中的实体有VoronoiComponent，则进入破碎模式
				m_VoronoiComponent = m_SelectedEntity->GetComponent<VoronoiComponent>();

				Application::Get().GetRenderSystem()->SetCurrentEntity(m_SelectedEntity);

				m_EngineMode = EngineMode::FractureMode;
			}
			else
			{
				// 若选中的实体没有VoronoiComponent，则进入编辑模式
				m_VoronoiComponent = nullptr;

				Application::Get().GetRenderSystem()->SetCurrentEntity(nullptr);

				m_EngineMode = EngineMode::EditMode;
			}
		}

		// 若开启破碎模式，则渲染Voronoi图
		if (m_EngineMode == EngineMode::FractureMode)
		{
			m_VoronoiFrameBuffer->Bind();

			glm::vec2 size = m_VoronoiFrameBuffer->GetSize();

			m_VoronoiComponent->SetViewportSize(size.x, size.y);
			
			Application::Get().GetRenderSystem()->Clear();

			Application::Get().GetRenderSystem()->RenderVoronoi(ts, m_VoronoiComponent);

			m_VoronoiFrameBuffer->Unbind();
		}
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_Scene->GetCamera()->OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(BS_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(BS_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}

	void EditorLayer::OnImGuiRender()
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

		if (ImGui::Button("Hello, world!"))
		{

			// TODO: 将切换投影类型的按钮写到合适的ImGui窗口中
			m_Scene->GetCamera()->SwitchProjectionType();
		}

		//m_Scene->OnImGuiRender();
		m_SceneHierarchyPanel.OnImGuiRender();

		ImGui::Begin("Scene");
		
		m_SceneFocused = ImGui::IsWindowFocused();
		m_SceneHovered = ImGui::IsWindowHovered();

		Application::Get().GetImGuiLayer().SetBlockEvents(!m_SceneHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_SceneViewportSize != *((glm::vec2*)&viewportPanelSize))
		{
			m_SceneViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
			m_SceneFrameBuffer->Resize((unsigned int)m_SceneViewportSize.x, (unsigned int)m_SceneViewportSize.y);
			m_SceneViewportSize = { m_SceneFrameBuffer->GetSpecification().width, m_SceneFrameBuffer->GetSpecification().height };
		}
		unsigned int textureID = m_SceneFrameBuffer->GetColorAttachmentRendererID();
		ImGui::Image((ImTextureID)(uintptr_t)textureID, ImVec2{ m_SceneViewportSize.x, m_SceneViewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));
		
		ImGui::End();

		if (m_EngineMode == EngineMode::FractureMode)
		{
			ImGui::Begin("Voronoi");
			
			ImVec2 voronoiPanelSize = ImGui::GetContentRegionAvail();

			m_VoronoiComponent->SetViewportSize(voronoiPanelSize.x, voronoiPanelSize.y);
			
			unsigned int voronoiTextureID = m_VoronoiFrameBuffer->GetColorAttachmentRendererID();

			float minXY = std::min(voronoiPanelSize.x, voronoiPanelSize.y);

			float offsetX = (voronoiPanelSize.x - minXY) * 0.5f;
			float offsetY = (voronoiPanelSize.y - minXY) * 0.5f;

			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offsetY);

			ImGui::Image((ImTextureID)(uintptr_t)voronoiTextureID, ImVec2{ minXY, minXY });
			
			ImGui::End();
		}
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		return false;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		return false;
	}

	bool EditorLayer::LoadFile(const std::string& filepath)
	{
		return false;
	}
}