#include "bspch.h"
#include "Scene/EmptyObject.h"

namespace BrokenSim
{
	EmptyObject::EmptyObject(unsigned int id, const std::string& name, Object* parent)
	{
		this->id = id;
		this->type = Type::Empty;
		this->SetParent(parent);
		this->SetName(name);
	}

	EmptyObject::~EmptyObject()
	{
		this->RemoveAllChildren();
	}

	void EmptyObject::OnUpdate(TimeStep ts, std::shared_ptr<Shader>& shader)
	{
	}

	void EmptyObject::OnRender(std::shared_ptr<Shader> shader)
	{
	}

	void EmptyObject::OnEvent(Event& e)
	{
	}
}
