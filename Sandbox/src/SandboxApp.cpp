#include "BrokenSimulation.h"
#include "Core/EntryPoint.h"

class Sanbox : public BrokenSim::Application
{
public:
	Sanbox()
	{
	}
	~Sanbox()
	{
	}
};

BrokenSim::Application* BrokenSim::CreateApplication()
{
	return new Sanbox();
}
