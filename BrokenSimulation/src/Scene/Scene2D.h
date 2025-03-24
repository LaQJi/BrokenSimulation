#pragma once

#include "Scene/Scene.h"

namespace BrokenSim
{
	class Scene2D : public Scene
	{
	public:
		Scene2D();
		~Scene2D();

		void OnUpdate(TimeStep ts, Camera& camera) override;
		void OnImGuiRender() override;
	private:
	};
