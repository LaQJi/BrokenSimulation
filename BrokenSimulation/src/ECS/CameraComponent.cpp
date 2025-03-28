#include "bspch.h"
#include "ECS/CameraComponent.h"
#include "CameraComponent.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace BrokenSim
{
	CameraComponent::CameraComponent(Entity* entity, float fov, float aspectRation, float nearClip, float farClip)
		: Component(entity)
		, m_FOV(fov),
		m_AspectRatio(aspectRation),
		m_NearClip(nearClip),
		m_FarClip(farClip)
	{
	}

	glm::mat4 CameraComponent::GetProjectionMatrix() const
	{
		return glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
	}

	glm::mat4 CameraComponent::GetViewMatrix() const
	{
		glm::mat4 rotation(1.0f);
		rotation = glm::rotate(rotation, m_Rotation.y, glm::vec3(0, 1, 0));
		rotation = glm::rotate(rotation, m_Rotation.x, glm::vec3(1, 0, 0));
		rotation = glm::rotate(rotation, m_Rotation.z, glm::vec3(0, 0, 1));

		glm::mat4 translation(1.0f);
		translation = glm::translate(translation, -m_Position);

		return rotation * translation;
	}

	void CameraComponent::SetFOV(float fov)
	{
		m_FOV = fov;
	}

	float CameraComponent::GetFOV() const
	{
		return m_FOV;
	}

	void CameraComponent::SetAspectRatio(float aspectRatio)
	{
		m_AspectRatio = aspectRatio;
	}

	float CameraComponent::GetAspectRatio() const
	{
		return m_AspectRatio;
	}

	void CameraComponent::SetNearClip(float nearClip)
	{
		m_NearClip = nearClip;
	}

	float CameraComponent::GetNearClip() const
	{
		return m_NearClip;
	}

	void CameraComponent::SetFarClip(float farClip)
	{
		m_FarClip = farClip;
	}

	float CameraComponent::GetFarClip() const
	{
		return m_FarClip;
	}
	
	void CameraComponent::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
	}

	glm::vec3 CameraComponent::GetPosition() const
	{
		return m_Position;
	}

	void CameraComponent::SetRotation(const glm::vec3& rotation)
	{
		m_Rotation = rotation;
	}

	glm::vec3 CameraComponent::GetRotation() const
	{
		return m_Rotation;
	}
}
