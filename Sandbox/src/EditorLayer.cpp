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
	}

	void EditorLayer::OnAttach()
	{
		m_SceneViewportSize = { 1280.0f, 720.0f };
		m_VoronoiViewportSize = { 1280.0f, 720.0f };

		FrameBufferSpecification sceneSpec;
		sceneSpec.width = 1280;
		sceneSpec.height = 720;
		sceneSpec.attachments = { {TextureFormat::RGBA8}, TextureFormat::DEPTH24STENCIL8 };
		m_SceneFrameBuffer = std::make_shared<FrameBuffer>(sceneSpec);

		FrameBufferSpecification voronoiSpec;
		voronoiSpec.width = 1280;
		voronoiSpec.height = 720;
		voronoiSpec.attachments = { {TextureFormat::RGBA8} };
		m_VoronoiFrameBuffer = std::make_shared<FrameBuffer>(voronoiSpec);

		m_Scene = std::make_shared<Scene>();

		Application::Get().GetRenderSystem()->SetCurrentShader("Phong");

		for (int i = 0; i < 8; i++)
		{
			std::stringstream ss;
			ss << "res/Models/ring_" << i << ".obj";
			Entity* entity = m_Scene->CreateEntity();
			entity->AddComponent<ModelComponent>(ss.str());
		}
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
		
		m_SceneFrameBuffer->Bind();

		Application::Get().GetRenderSystem()->SetClearColor({ 0.25f, 0.25f, 0.25f, 1.0f });
		Application::Get().GetRenderSystem()->Clear();
		
		Application::Get().GetRenderSystem()->OnUpdate(ts, m_Scene.get());

		m_SceneFrameBuffer->Unbind();

		// Èô¿ªÆôÆÆËéÄ£Ê½£¬ÔòäÖÈ¾VoronoiÍ¼
		if (m_EngineMode == EngineMode::FractureMode)
		{
			m_VoronoiFrameBuffer->Bind();
			
			Application::Get().GetRenderSystem()->Clear();

			if (!m_VoronoiComponent)
			{
				m_VoronoiComponent = new VoronoiComponent(nullptr, 10);
			}
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