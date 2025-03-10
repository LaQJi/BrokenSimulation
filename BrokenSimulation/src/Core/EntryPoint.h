#pragma once

#include "Core/Config.h"
#include "Core/Application.h"

#ifdef BS_PLATFORM_WINDOWS

extern BrokenSim::Application* BrokenSim::CreateApplication();

int main(int argc, char** argv)
{
	BrokenSim::Log::Init();
	BS_CORE_WARN("Initialized Log!");
	int a = 5;
	BS_INFO("Hello! Var={0}", a);
	auto app = BrokenSim::CreateApplication();
	app->Run();
	delete app;
}

#endif
