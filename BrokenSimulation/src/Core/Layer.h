#pragma once

#include "bspch.h"
#include "Core/TimeStep.h"
#include "Events/Event.h"

namespace BrokenSim
{
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer")
			: layerName(name) {
		}
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}

		virtual void OnUpdate(TimeStep ts) {}
		virtual void OnEvent(Event& event) {}
		virtual void OnImGuiRender() {}

		inline const std::string& GetName() const { return layerName; }

	protected:
		std::string layerName;
	};
}
