#include "EditorLayer.h"

#include <glew/include/GL/glew.h>
#include <GLFW/include/GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ImGuizmo/ImGuizmo.h>

#include "Geometry/Math.h"

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

		Application::Get().GetRenderSystem()->SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Application::Get().GetRenderSystem()->Clear();
		
		Application::Get().GetRenderSystem()->OnUpdate(ts, m_Scene.get());

		m_SceneFrameBuffer->Unbind();

		m_SelectedEntity = m_SceneHierarchyPanel.GetSelectionContext();

		// ����ѡ�е�ʵ�壬�����ʵ���Ƿ���VoronoiComponent�л�����״̬
		if (m_SelectedEntity)
		{
			if (m_SelectedEntity->HasComponent<VoronoiComponent>())
			{
				// ��ѡ�е�ʵ����VoronoiComponent�����������ģʽ
				m_VoronoiComponent = m_SelectedEntity->GetComponent<VoronoiComponent>();

				Application::Get().GetRenderSystem()->SetCurrentEntity(m_SelectedEntity);

				m_EngineMode = EngineMode::FractureMode;
			}
			else
			{
				// ��ѡ�е�ʵ��û��VoronoiComponent�������༭ģʽ
				m_VoronoiComponent = nullptr;

				Application::Get().GetRenderSystem()->SetCurrentEntity(nullptr);

				m_EngineMode = EngineMode::EditMode;
			}
		}

		/*
		// ����������ģʽ������ȾVoronoiͼ
		if (m_EngineMode == EngineMode::FractureMode)
		{
			m_VoronoiFrameBuffer->Bind();

			glm::vec2 size = m_VoronoiFrameBuffer->GetSize();

			m_VoronoiComponent->SetViewportSize(size.x, size.y);
			
			Application::Get().GetRenderSystem()->Clear();

			Application::Get().GetRenderSystem()->RenderVoronoi(ts, m_VoronoiComponent);

			m_VoronoiFrameBuffer->Unbind();
		}
		*/
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
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);

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
				if (ImGui::MenuItem("Import Model", "Ctrl+N"))
				{
					OPENFILENAMEA ofn;

					char fileName[MAX_PATH] = "";

					ZeroMemory(&ofn, sizeof(ofn));
					ofn.lStructSize = sizeof(ofn);
					ofn.lpstrFile = fileName;
					ofn.nMaxFile = MAX_PATH;
					ofn.lpstrFilter = "Model Files\0*.obj;*.fbx;*.gltf\0";
					ofn.nFilterIndex = 1;
					ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

					if (GetOpenFileNameA(&ofn))
					{
						std::string name = std::string(fileName);
						name = name.substr(name.find_last_of("/\\") + 1, name.length() - name.find_last_of("/\\") - 1);
						name = name.substr(0, name.find_last_of('.'));
						Entity* entity = m_Scene->CreateEntity(name);
						entity->AddComponent<ModelComponent>(fileName);
					}
				}

				if (ImGui::MenuItem("Perspective/Orthogonal"))
				{
					m_Scene->GetCamera()->SwitchProjectionType();
				}

				if (ImGui::MenuItem("Exit"))
				{
					Application::Get().Close();
				}
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

		//ImGui::ShowDemoWindow();

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


		//ImGuizmo::BeginFrame();
		// ImGuizmos
		m_SelectedEntity = m_SceneHierarchyPanel.GetSelectionContext();

		if (m_SelectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist(ImGui::GetWindowDrawList());

			float windowWidth = (float)ImGui::GetWindowContentRegionMax().x -
				(float)ImGui::GetWindowContentRegionMin().x;

			float windowHeight = (float)ImGui::GetWindowContentRegionMax().y -
				(float)ImGui::GetWindowContentRegionMin().y;

			ImGuizmo::SetRect(ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMin().x,
				ImGui::GetWindowPos().y + ImGui::GetWindowContentRegionMin().y,
				windowWidth, windowHeight);

			// Camera
			const auto& camera = m_Scene->GetCameraRef();
			const glm::mat4& view = camera.GetViewMatrix();
			const glm::mat4& projection = camera.GetProjectionMatrix();

			// Entity
			glm::mat4 transform = m_SelectedEntity->GetModelMatrix();

			// Snapping
			bool snap = Input::IsKeyPressed(Key::LeftControl);
			float snapValue = 0.5f;

			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
			{
				snapValue = 45.0f;
			}

			float snapValues[3] = { snapValue, snapValue, snapValue };

			// �������
			ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection),
				(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::MODE::LOCAL,
				glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);

			// ����ʵ��ı任
			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;

				ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform),
					glm::value_ptr(translation), glm::value_ptr(rotation), glm::value_ptr(scale));

				// ����תת��Ϊŷ����
				rotation.x = glm::degrees(rotation.x);
				rotation.y = glm::degrees(rotation.y);
				rotation.z = glm::degrees(rotation.z);

				
				m_SelectedEntity->GetPosition() = translation;
				m_SelectedEntity->GetRotation() = rotation;
				m_SelectedEntity->GetScale() = scale;

			}
		}



		
		ImGui::End();

		/*
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
		*/
		ImGui::End();
	}

	void EditorLayer::SetSelected(Entity* entity)
	{
		m_SelectedEntity = entity;
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		// ��ȡctrl��״̬
		bool control = Input::IsKeyPressed(Key::LeftControl) ||
			Input::IsKeyPressed(Key::RightControl);

		// ��ȡshift��״̬
		bool shift = Input::IsKeyPressed(Key::LeftShift) ||
			Input::IsKeyPressed(Key::RightShift);

		switch (e.GetKeyCode())
		{
		case Key::N:
		{
			if (control)
			{
				OPENFILENAMEA ofn;

				char fileName[MAX_PATH] = "";

				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.lpstrFile = fileName;
				ofn.nMaxFile = MAX_PATH;
				ofn.lpstrFilter = "Model Files\0*.obj;*.fbx;*.gltf\0";
				ofn.nFilterIndex = 1;
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

				if (GetOpenFileNameA(&ofn))
				{
					std::string name = std::string(fileName);
					name = name.substr(name.find_last_of("/\\") + 1, name.length() - name.find_last_of("/\\") - 1);
					name = name.substr(0, name.find_last_of('.'));
					Entity* entity = m_Scene->CreateEntity(name);
					entity->AddComponent<ModelComponent>(fileName);
				}
			}
			break;
		}
		case Key::Q:
		{
			if (!ImGuizmo::IsUsing())
				m_GizmoType = -1;
			break;
		}
		case Key::W:
		{
			if (!ImGuizmo::IsUsing())
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;
		}
		case Key::E:
		{
			if (!ImGuizmo::IsUsing())
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;
			break;
		}
		case Key::R:
		{
			if (!ImGuizmo::IsUsing())
				m_GizmoType = ImGuizmo::OPERATION::SCALE;
			break;
		}
		}

		return false;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		/*
		if (e.GetButton() == Mouse::Button_Left)
		{
			if (m_SceneHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
			{
				m_SceneHierarchyPanel.SetSelectionContext(nullptr);
			}
		}
		*/

		return false;
	}

	bool EditorLayer::LoadFile(const std::string& filepath)
	{
		return false;
	}
}