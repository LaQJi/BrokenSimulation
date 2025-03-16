#pragma once

#include "Core/Window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace BrokenSim
{
	static int glfwWindowCount = 0;

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props = WindowProps());
		~WindowsWindow();

		void OnUpdate();

		void SetEventCallback(const EventCallbackFunc& callback);

		void SetVSync(bool enabled);

		bool IsVSync() const;

		virtual unsigned int GetWidth() const override;
		virtual unsigned int GetHeight() const override;

		void* GetNativeWindow() const;

	private:
		struct WindowData
		{
			std::string title;
			unsigned int width, height;
			bool VSync;

			EventCallbackFunc eventCallback;
		};
		WindowData data;

		GLFWwindow* window;

		void Init(const WindowProps& props);
		void Destroy();
	};
}
