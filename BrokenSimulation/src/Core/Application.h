#pragma once

#include <memory>

#include "Core/Config.h"
#include "Core/Window.h"
#include "Core/Log.h"
#include "Events/ApplicationEvent.h"

namespace BrokenSim
{
	class BS_API Application
	{
	public:
		Application();
		virtual ~Application();

		//void OnEvent();

		void PushLayer();
		void PushOverlay();

		//Window& GetWindow() { return *m_Window; }

		void Run();

		void Close();

		static Application& Get() { return *s_Instance; }

	private:
		//bool OnWindowClose(WindowCloseEvent& e);
		//bool OnWindowResize(WindowResizeEvent& e);

	private:
		//std::unique_ptr<Window> m_Window;
		bool m_Running = true;

	private:
		static Application* s_Instance;
	};

	// 需在客户端定义
	Application* CreateApplication();
}