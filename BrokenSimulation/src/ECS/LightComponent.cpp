#include "bspch.h"
#include "ECS/LightComponent.h"

namespace BrokenSim
{
	LightComponent::LightComponent(Entity* owner, LightType type, glm::vec3 color, glm::vec3 position)
		: Component(owner), m_Type(type), m_Color(color), m_Position(position)
	{
	}
}
