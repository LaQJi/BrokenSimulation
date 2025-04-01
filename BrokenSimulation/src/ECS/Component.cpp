#include "bspch.h"
#include "ECS/Component.h"

#include <glm/gtc/matrix_transform.hpp>

namespace BrokenSim
{
	const glm::mat4& Component::GetTransform() const
	{
		glm::mat4 transform = glm::mat4(1.0f);

		transform = glm::translate(glm::mat4(1.0f), m_Position) * transform;

		transform = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) * transform;
		transform = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) * transform;
		transform = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) * transform;

		transform = glm::scale(glm::mat4(1.0f), m_Scale) * transform;

		return transform;
	}

	glm::vec3& Component::GetPosition()
	{
		return m_Position;
	}

	void Component::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
	}

	glm::vec3& Component::GetRotation()
	{
		return m_Rotation;
	}

	void Component::SetRotation(const glm::vec3& rotation)
	{
		m_Rotation = rotation;
	}

	glm::vec3& Component::GetScale()
	{
		return m_Scale;
	}

	void Component::SetScale(const glm::vec3& scale)
	{
		m_Scale = scale;
	}
}
