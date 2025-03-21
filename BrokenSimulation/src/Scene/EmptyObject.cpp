#include "bspch.h"
#include "Scene/EmptyObject.h"

namespace BrokenSim
{
	EmptyObject::EmptyObject(unsigned int id, Object* parent, const std::string& name)
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

	void EmptyObject::OnUpdate(TimeStep ts)
	{
	}

	void EmptyObject::OnRender()
	{
	}

	void EmptyObject::OnEvent(Event& e)
	{
	}
}
