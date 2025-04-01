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
		// 该帧缓冲用于渲染场景
		std::shared_ptr<FrameBuffer> m_SceneFrameBuffer;
		// 该帧缓冲用于渲染Voronoi图，仅在破碎模式下使用
		std::shared_ptr<FrameBuffer> m_VoronoiFrameBuffer;

		// 场景
		std::shared_ptr<Scene> m_Scene;

		// 当前选中的对象
		Entity* m_SelectedEntity;
		// 当前选中的对象的Voronoi图组件
		VoronoiComponent* m_VoronoiComponent = nullptr;

		// 当前用于渲染帧缓冲的ImGui窗口大小
		glm::vec2 m_SceneViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_VoronoiViewportSize = { 0.0f, 0.0f };

		// 当前鼠标是否悬停在场景窗口上
		bool m_SceneHovered = false;
		// 当前场景窗口是否被聚焦
		bool m_SceneFocused = false;

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

		// 场景层次面板
		SceneHierarchyPanel m_SceneHierarchyPanel;
	};
}
