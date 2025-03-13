#include "bspch.h"
#include "Core/Macros.h"
#include "Core/Application.h"
#include "Core/Log.h"
#include "Events/ApplicationEvent.h"
#include "Utils/Utils.h"

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
		m_Window = Window::Create();
		m_Window->SetEventCallback(BS_BIND_EVENT_FN(Application::OnEvent));
		m_ImGuiLayer = new ImGuiLayer();
		m_ImGuiLayer->OnAttach();
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
		// 分发事件给Layer，后续若有多层Layer，需要分发给每一层，同时需要考虑事件的传递
		m_ImGuiLayer->OnEvent(e);
	}

	void Application::PushLayer()
	{
	}

	void Application::PushOverlay()
	{
	}

	Window& Application::GetWindow()
	{
		return *m_Window;
	}

	ImGuiLayer& Application::GetImGuiLayer()
	{
		return *m_ImGuiLayer;
	}

	void Application::Run()
	{
		while (m_Running)
		{
			float time = Time::GetTime();
			TimeStep ts = time - m_lastFrameTime;
			m_lastFrameTime = time;

			if (!m_Minimized)
			{
				m_ImGuiLayer->Begin();

				// 渲染Layer
				{
					m_ImGuiLayer->OnImGuiRender();
				}

				m_ImGuiLayer->End();
			}

			m_Window->OnUpdate();
		}
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
}