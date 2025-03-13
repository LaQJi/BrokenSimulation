#pragma once

#include "Core/Config.h"
#include "Core/Window.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Platform/ImGuiLayer.h"

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
		ImGuiLayer& GetImGuiLayer();

		void Run();

		void Close();

		static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;

		float m_lastFrameTime = 0.0f;

		bool m_Running = true;
		bool m_Minimized = false;

	private:
		static Application* s_Instance;
	};

	// ���ڿͻ��˶���
	Application* CreateApplication();
}