#pragma once

#include "Core/Config.h"
#include "Core/Window.h"
#include "Core/Log.h"
#include "Events/Event.h"

namespace BrokenSim
{
	class BS_API Application
	{
	public:
		Application();
		virtual ~Application();

		void OnEvent(Event& e);

		void PushLayer();
		void PushOverlay();

		Window& GetWindow();

		void Run();

		void Close();

		static Application& Get() { return *s_Instance; }

	private:
		//bool OnWindowClose(WindowCloseEvent& e);
		//bool OnWindowResize(WindowResizeEvent& e);

	private:
		std::unique_ptr<Window> window;
		bool m_Running = true;

	private:
		static Application* s_Instance;
	};

	// 需在客户端定义
	Application* CreateApplication();
}