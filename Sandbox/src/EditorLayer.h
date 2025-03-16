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

		// UI Panels
		void UI_Toolbar();

	private:
		std::shared_ptr<VertexArray> m_SquareVA;
		std::shared_ptr<Shader> m_FlatColorShader;

		enum class SceneState
		{
			Edit = 0, Process = 1
		};
		SceneState m_SceneState = SceneState::Edit;
	};
}
