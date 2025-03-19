#include "bspch.h"
#include "Platform/WindowsWindow.h"
#include "Core/Log.h"
#include "Core/Macros.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace BrokenSim
{
	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		// 初始化GLFW
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Destroy();
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	void WindowsWindow::SetEventCallback(const EventCallbackFunc& callback)
	{
		data.eventCallback = callback;
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}
		data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return data.VSync;
	}

	unsigned int WindowsWindow::GetWidth() const
	{
		return data.width;
	}

	unsigned int WindowsWindow::GetHeight() const
	{
		return data.height;
	}

	void* WindowsWindow::GetNativeWindow() const
	{
		return window;
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		data.title = props.title;
		data.width = props.width;
		data.height = props.height;

		BS_CORE_INFO("Creating window {0} ({1}, {2})", props.title, props.width, props.height);

		if (glfwWindowCount == 0)
		{
			int success = glfwInit();
			BS_CORE_ASSERT(success, "Failed to initialize GLFW!");
			glfwSetErrorCallback([](int error, const char* description)
				{
					BS_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
				});
		}

		{
			// 创建窗口并创建OpenGL上下文
			window = glfwCreateWindow((int)props.width, (int)props.height, props.title.c_str(), nullptr, nullptr);

			if (!window)
			{
				BS_CORE_ERROR("Failed to create GLFW window!");
				glfwTerminate();
				return;
			}

			glfwWindowCount++;
		}

		// 使OpenGL的上下文为当前窗口的上下文
		glfwMakeContextCurrent(window);

		// 初始化GLEW
		int success = glewInit();
		BS_CORE_ASSERT(success == GLEW_OK, "Failed to initialize GLEW!");

		glfwSetWindowUserPointer(window, &data);
		SetVSync(true);

		// 设置回调函数
		glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.width = width;
				data.height = height;

				WindowResizeEvent event(width, height);
				data.eventCallback(event);
			});

		glfwSetWindowCloseCallback(window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				WindowCloseEvent event;
				data.eventCallback(event);
			});

		glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, false);
					data.eventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.eventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, true);
					data.eventCallback(event);
					break;
				}
				}
			});

		glfwSetCharCallback(window, [](GLFWwindow* window, unsigned int keycode)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				KeyTypedEvent event(keycode);
				data.eventCallback(event);
			});

		glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.eventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.eventCallback(event);
					break;
				}
				}
			});

		glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent event((float)xPos, (float)yPos);
				data.eventCallback(event);
			});

		glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data.eventCallback(event);
			});
	}

	void WindowsWindow::Destroy()
	{
		glfwDestroyWindow(window);
		glfwWindowCount--;
		if (glfwWindowCount == 0)
		{
			glfwTerminate();
		}
	}
}
