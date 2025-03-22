#pragma once

#include "Scene/Object.h"

namespace BrokenSim
{
	class EmptyObject : public Object
	{
	public:
		EmptyObject(unsigned int id, const std::string& name = "Empty", Object* parent = nullptr);
		virtual ~EmptyObject() override;

		virtual void OnUpdate(TimeStep ts, std::shared_ptr<Shader> shader) override;

		virtual void OnRender(std::shared_ptr<Shader> shader) override;

		virtual void OnEvent(Event& e) override;

	private:
	};
}
