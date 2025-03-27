#pragma once

#include "Core/Config.h"
#include "Core/Application.h"
#include "Core/Log.h"

#ifdef BS_PLATFORM_WINDOWS

extern BrokenSim::Application* BrokenSim::CreateApplication();

int main(int argc, char** argv)
{
	BrokenSim::Log::Init();
	auto app = BrokenSim::CreateApplication();
	app->Run();
	delete app;
}

#endif
