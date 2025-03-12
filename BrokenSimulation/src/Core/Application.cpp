#include "bspch.h"
#include "Core/Application.h"

namespace BrokenSim
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		//m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{
	}

	void Application::PushLayer()
	{
	}

	void Application::PushOverlay()
	{
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		BS_CORE_INFO(e);
		while (m_Running)
		{
			//m_Window->OnUpdate();
		}
	}
}