#pragma once

#include "Events/Event.h"

namespace BrokenSim
{
	struct WindowProps
	{
		std::string title;
		unsigned int width;
		unsigned int height;

		WindowProps(const std::string& title = "Broken Simulation",
			unsigned int width = 1280,
			unsigned int height = 720)
			: title(title), width(width), height(height)
		{
		}
	};

	class Window
	{
	public:
		using EventCallbackFunc = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual void SetEventCallback(const EventCallbackFunc& callback) = 0;

		virtual void SetVSync(bool enabled) = 0;

		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static std::unique_ptr<Window> Create(const WindowProps& props = WindowProps());
	};
}
