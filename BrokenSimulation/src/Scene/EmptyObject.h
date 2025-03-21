#pragma once

#include "Scene/Object.h"

namespace BrokenSim
{
	class EmptyObject : public Object
	{
	public:
		EmptyObject(unsigned int id, Object* parent = nullptr, const std::string& name = "Empty");
		virtual ~EmptyObject() override;

		virtual void OnUpdate(TimeStep ts) override;

		virtual void OnRender() override;

		virtual void OnEvent(Event& e) override;

	private:
	};
}
