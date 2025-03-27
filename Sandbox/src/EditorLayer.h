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
		// 该帧缓冲用于渲染场景
		std::shared_ptr<FrameBuffer> sceneFrameBuffer;
		// 该帧缓冲用于渲染Voronoi图，仅在破碎模式下使用
		std::shared_ptr<FrameBuffer> voronoiFrameBuffer;

		// 场景
		std::shared_ptr<Scene3D> m_Scene;
		std::shared_ptr<Voronoi2DScene> m_VoronoiScene;

		// 当前活动的场景
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		// 当前鼠标是否悬停在窗口上
		bool m_Hovered = false;
		// 当前窗口是否被聚焦
		bool m_Focused = false;

		enum class EngineMode
		{
			FractureMode = 0,
			EditMode = 1
		};
		// 当前场景状态
		EngineMode m_EngineMode = EngineMode::FractureMode;

		enum class Language
		{
			English = 0,
			Chinese = 1
		};
		// 当前语言
		Language m_Language = Language::English;

		// 字体路径
		std::string m_FontPath;

		// 当前工作目录
		std::string m_WorkingDirectory;
	};
}
