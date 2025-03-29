#include "BrokenSimulation.h"
#include "Core/EntryPoint.h"

#include "EditorLayer.h"

namespace BrokenSim
{
	class Sanbox : public Application
	{
	public:
		Sanbox()
		{
			PushLayer(new EditorLayer());
		}
		~Sanbox()
		{
		}
	};

	Application* CreateApplication()
	{
		return new Sanbox();
	}
}
