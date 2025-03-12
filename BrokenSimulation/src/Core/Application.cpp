#include "bspch.h"
#include "Core/Macros.h"
#include "Core/Application.h"

namespace BrokenSim
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		if (!s_Instance)
		{
			s_Instance = this;
		}
		else
		{
			BS_CORE_ERROR("Application already exists!");
		}
		window = Window::Create();
		window->SetEventCallback(BS_BIND_EVENT_FN(Application::OnEvent));
	}

	Application::~Application()
	{
	}

	void Application::OnEvent(Event& e)
	{
		BS_CORE_INFO(e);
	}

	void Application::PushLayer()
	{
	}

	void Application::PushOverlay()
	{
	}

	Window& Application::GetWindow()
	{
		return *window;
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::Run()
	{
		while (m_Running)
		{
			window->OnUpdate();
		}
	}
}