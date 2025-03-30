#pragma once

#include "BrokenSimulation.h"

namespace BrokenSim
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const std::shared_ptr<Scene>& context);

		std::shared_ptr<Scene> GetContext() const;
		void SetContext(const std::shared_ptr<Scene>& context);

		void OnImGuiRender();

		Entity* GetSelectionContext() const;
		void SetSelectionContext(const Entity* context);

	private:
		void DrawEntityNode(Entity* entity);
		void DrawComponents(Entity* entity);

	private:
		std::shared_ptr<Scene> m_Context;
		Entity* m_SelectionContext;
	};
}