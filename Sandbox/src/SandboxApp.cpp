#include "BrokenSimulation.h"
#include "Core/EntryPoint.h"

#include "TestLayer.h"
#include "EditorLayer.h"

namespace BrokenSim
{
	class Sanbox : public Application
	{
	public:
		Sanbox()
		{
			//PushLayer(new TestLayer());
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
