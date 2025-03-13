#pragma once

#include "Core/Layer.h"

namespace BrokenSim
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnEvent(Event& event) override;

		//void SetLightTheme();
		//void SetDarkTheme();
		//void SetClassicTheme();

		void Begin();
		void End();
	};
}
