#pragma once

#include "BrokenSimulation.h"

#include "panels/SceneHierarchyPanel.h"

namespace BrokenSim
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(TimeStep ts) override;
		virtual void OnEvent(Event& e) override;
		virtual void OnImGuiRender() override;

	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		bool LoadFile(const std::string& filepath);


	private:
		// ��֡����������Ⱦ����
		std::shared_ptr<FrameBuffer> m_SceneFrameBuffer;
		// ��֡����������ȾVoronoiͼ����������ģʽ��ʹ��
		std::shared_ptr<FrameBuffer> m_VoronoiFrameBuffer;

		// ����
		std::shared_ptr<Scene> m_Scene;

		// ��ǰѡ�еĶ���
		Entity* m_SelectedEntity;
		// ��ǰѡ�еĶ����Voronoiͼ���
		VoronoiComponent* m_VoronoiComponent = nullptr;

		// ��ǰ������Ⱦ֡�����ImGui���ڴ�С
		glm::vec2 m_SceneViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_VoronoiViewportSize = { 0.0f, 0.0f };

		// ��ǰ����Ƿ���ͣ�ڳ���������
		bool m_SceneHovered = false;
		// ��ǰ���������Ƿ񱻾۽�
		bool m_SceneFocused = false;

		enum class EngineMode
		{
			FractureMode = 0,
			EditMode = 1
		};
		// ��ǰ����״̬
		EngineMode m_EngineMode = EngineMode::FractureMode;

		enum class Language
		{
			English = 0,
			Chinese = 1
		};
		// ��ǰ����
		Language m_Language = Language::English;

		// ����·��
		std::string m_FontPath;

		// ��ǰ����Ŀ¼
		std::string m_WorkingDirectory;

		// ����������
		SceneHierarchyPanel m_SceneHierarchyPanel;
	};
}
