#pragma once

namespace BrokenSim
{
	class Window
	{
	public:
		Window();
		~Window();

		void OnUpdate();
		void OnRender();
		void OnImGuiRender();
	};
}
