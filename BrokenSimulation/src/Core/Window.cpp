#include "bspch.h"
#include "Core/Window.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Platform/WindowsWindow.h"

namespace BrokenSim
{
	std::unique_ptr<Window> Window::Create(const WindowProps& props)
	{
		return std::make_unique<WindowsWindow>(props);
	}
}
