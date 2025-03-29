#pragma once

#include "Core/Config.h"
#include "Core/Window.h"
#include "Core/Layer.h"
#include "Core/LayerStack.h"
#include "Core/ResourceManager.h"
#include "ECS/RenderSystem.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Platform/ImGuiLayer.h"

namespace BrokenSim
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		Window& GetWindow();
		ImGuiLayer& GetImGuiLayer();

		void Run();

		void Close();

		static Application& Get() { return *s_Instance; }

		std::shared_ptr<RenderSystem> GetRenderSystem() { return m_RenderSystem; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	protected:
		std::shared_ptr<ResourceManager> m_ResourceManager;
		std::shared_ptr<RenderSystem> m_RenderSystem;

	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;

		LayerStack m_LayerStack;

		float m_LastFrameTime = 0.0f;

		bool m_Running = true;
		bool m_Minimized = false;


		unsigned int va, vb, ib;

	private:
		static Application* s_Instance;
	};

	// 需在客户端定义
	Application* CreateApplication();
}