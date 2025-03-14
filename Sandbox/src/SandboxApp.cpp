#include "BrokenSimulation.h"
#include "Core/EntryPoint.h"

#include "TestLayer.h"

class Sanbox : public BrokenSim::Application
{
public:
	Sanbox()
	{
		PushLayer(new TestLayer());
	}
	~Sanbox()
	{
	}
};

BrokenSim::Application* BrokenSim::CreateApplication()
{
	return new Sanbox();
}
