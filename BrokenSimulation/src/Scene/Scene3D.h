#pragma once

#include "Scene/Scene.h"

namespace BrokenSim
{
	class Scene3D : public Scene
	{
	public:
		Scene3D();
		virtual ~Scene3D();

		virtual void OnUpdate(TimeStep ts, Camera& camera) override;
		virtual void OnRender() override;

		virtual void OnImGuiRender() override;
	};
}
