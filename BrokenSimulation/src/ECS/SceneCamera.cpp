#include "bspch.h"
#include "ECS/SceneCamera.h"

#include "Core/Input.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace BrokenSim
{
	SceneCamera::SceneCamera(ProjectionType type, float initialValue, float aspectRatio, float nearClip, float farClip)
	{
		m_ProjectionType = type;
		SetProjectionMatrix(initialValue, aspectRatio, nearClip, farClip);
		UpdateAlternateValue();
	}

	void SceneCamera::OnUpdate(TimeStep ts)
	{
		if (Input::IsKeyPressed(Key::LeftAlt))
		{
			const glm::vec2& mouse = Input::GetMousePosition();
			// 计算鼠标移动距离
			glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
			m_InitialMousePosition = mouse;

			if (Input::IsMouseButtonPressed(Mouse::Button_Middle))
			{
				MousePan(delta);
			}
			else if (Input::IsMouseButtonPressed(Mouse::Button_Left))
			{
				MouseRotate(delta);
			}
			else if (Input::IsMouseButtonPressed(Mouse::Button_Right))
			{
				MouseZoom(delta.y);
			}
		}

		UpdateView();
	}

	void SceneCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BS_BIND_EVENT_FN(SceneCamera::OnMouseScroll));
	}

	void SceneCamera::SwitchProjectionType()
	{
		UpdateAlternateValue();
		if (m_ProjectionType == ProjectionType::Perspective)
		{
			m_ProjectionType = ProjectionType::Orthographic;
		}
		else if (m_ProjectionType == ProjectionType::Orthographic)
		{
			m_ProjectionType = ProjectionType::Perspective;
		}
		else
		{
			BS_CORE_ASSERT(false, "Unknown projection type!");
		}
	}

	void SceneCamera::SetViewportSize(float width, float height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;
		UpdateProjection();
	}

	void SceneCamera::SetProjectionMatrix(float typeValue, float aspectRatio, float nearClip, float farClip)
	{
		if (m_ProjectionType == ProjectionType::Perspective)
		{
			m_FOV = glm::clamp(typeValue, 1.0f, 179.0f);
			m_AspectRatio = aspectRatio;
			m_NearClip = nearClip;
			m_FarClip = farClip;
			// 计算透视投影矩阵
			m_ProjectionMatrix = glm::perspective(
									glm::radians(m_FOV),
									m_AspectRatio,
									m_NearClip,
									m_FarClip);
		}
		else
		{
			m_OrthoSize = std::max(typeValue, 0.01f);
			m_AspectRatio = aspectRatio;
			m_NearClip = nearClip;
			m_FarClip = farClip;

			float halfHeight = m_OrthoSize * 0.5f;
			float halfWidth = halfHeight * m_AspectRatio;
			// 计算正交投影矩阵
			m_ProjectionMatrix = glm::ortho(
									-halfWidth, halfWidth,
									-halfHeight, halfHeight,
									m_NearClip,
									m_FarClip);
		}
	}

	glm::vec3 SceneCamera::GetUpDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 SceneCamera::GetRightDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 SceneCamera::GetForwardDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::quat SceneCamera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
	}

	bool SceneCamera::OnMouseScroll(MouseScrolledEvent& e)
	{
		float delta = e.GetYOffset() * 0.1f;
		MouseZoom(delta);
		UpdateView();
		return false;
	}

	void SceneCamera::UpdateProjection()
	{
		// 更新宽高比
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		// 更新投影矩阵
		if (m_ProjectionType == ProjectionType::Perspective)
		{
			m_ProjectionMatrix = glm::perspective(
					glm::radians(m_FOV),
					m_AspectRatio,
					m_NearClip,
					m_FarClip);
		}
		else
		{
			float halfHeight = m_OrthoSize * 0.5f;
			float halfWidth = halfHeight * m_AspectRatio;
			m_ProjectionMatrix = glm::ortho(
					-halfWidth, halfWidth,
					-halfHeight, halfHeight,
					m_NearClip,
					m_FarClip);
		}
		// 更新视图投影矩阵
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void SceneCamera::UpdateView()
	{
		m_Position = CalculatePosition();

		// 更新视图矩阵
		glm::quat orientation = GetOrientation();
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
		m_ViewMatrix = glm::inverse(m_ViewMatrix);
		// 更新视图投影矩阵
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	glm::vec3 SceneCamera::CalculatePosition()
	{
		return m_FocalPoint - GetForwardDirection() * m_Distance;
	}

	std::pair<float, float> SceneCamera::PanSpeed() const
	{
		// 计算平移速度，根据视口大小调整
		float x = std::min(m_ViewportWidth / 1000.0f, 2.4f);
		float xFactor = 0.0366f * x * x - 0.1778f * x + 0.3021f;

		float y = std::min(m_ViewportHeight / 1000.0f, 2.4f);
		float yFactor = 0.0366f * y * y - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float SceneCamera::RotationSpeed() const
	{
		return 0.8f;
	}

	float SceneCamera::ZoomSpeed() const
	{
		// 计算缩放速度
		float distance = m_Distance * 0.2f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f);

		return speed;
	}

	void SceneCamera::MousePan(const glm::vec2& delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		// 计算平移距离
		m_FocalPoint += -GetRightDirection() * delta.x * xSpeed * m_Distance;
		m_FocalPoint += GetUpDirection() * delta.y * ySpeed * m_Distance;
	}

	void SceneCamera::MouseRotate(const glm::vec2& delta)
	{
		// 计算旋转角度
		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		m_Yaw += yawSign * delta.x * RotationSpeed();
		m_Pitch += delta.y * RotationSpeed();
	}

	void SceneCamera::MouseZoom(float delta)
	{
		// 计算缩放距离
		m_Distance -= delta * ZoomSpeed();
		if (m_Distance < 1.0f)
		{
			m_FocalPoint += GetForwardDirection();
			m_Distance = 1.0f;
		}
	}

	void SceneCamera::UpdateAlternateValue()
	{
		if (m_ProjectionType == ProjectionType::Perspective)
		{
			m_OrthoSize = m_Distance * tan(glm::radians(m_FOV * 0.5f));
		}
		else
		{
			m_FOV = glm::degrees(2.0f * atan(m_OrthoSize / m_Distance));
		}
	}
}
