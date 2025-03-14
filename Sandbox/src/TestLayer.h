#pragma once

#include "BrokenSimulation.h"
#include "imgui/imgui.h"

class TestLayer : public BrokenSim::Layer
{
public:
	TestLayer()
		: Layer("TestLayer")
	{
	}

	void OnUpdate(BrokenSim::TimeStep ts) override
	{
	}

	void OnEvent(BrokenSim::Event& event) override
	{
		BS_CORE_DEBUG(event);
	}

	void OnImGuiRender() override
	{
		ImGui::ShowDemoWindow();
	}
};