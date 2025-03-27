#pragma once

#include "BrokenSimulation.h"

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
		std::shared_ptr<FrameBuffer> sceneFrameBuffer;
		// ��֡����������ȾVoronoiͼ����������ģʽ��ʹ��
		std::shared_ptr<FrameBuffer> voronoiFrameBuffer;

		// ����
		std::shared_ptr<Scene3D> m_Scene;
		std::shared_ptr<Voronoi2DScene> m_VoronoiScene;

		// ��ǰ��ĳ���
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		// ��ǰ����Ƿ���ͣ�ڴ�����
		bool m_Hovered = false;
		// ��ǰ�����Ƿ񱻾۽�
		bool m_Focused = false;

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
	};
}
