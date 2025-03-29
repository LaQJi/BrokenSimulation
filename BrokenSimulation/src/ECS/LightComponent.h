#pragma once

#include "ECS/Component.h"

namespace BrokenSim
{
	class LightComponent : public Component
	{
	public:
		enum class LightType
		{
			Point		= 0,	// 点光源
			Directional	= 1		// 平行光
		};

		LightComponent(Entity* owner, LightType type = LightType::Point, glm::vec3 color = glm::vec3(1.0f), glm::vec3 position = glm::vec3(0.0f));

		glm::vec3& GetColor() { return m_Color; }
		glm::vec3& GetPosition() { return m_Position; }
		LightType GetType() { return m_Type; }


	private:
		LightType m_Type;
		glm::vec3 m_Color;
		glm::vec3 m_Position;
	};
}
