#include "bspch.h"
#include "Core/Macros.h"
#include "Core/Application.h"
#include "Core/Log.h"
#include "Events/ApplicationEvent.h"

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
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BS_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BS_BIND_EVENT_FN(Application::OnWindowResize));

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

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		return false;
	}

	void Application::Run()
	{
		while (m_Running)
		{
			window->OnUpdate();
		}
	}
}