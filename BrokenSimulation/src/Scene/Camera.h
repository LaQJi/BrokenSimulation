#pragma once

#include "Core/TimeStep.h"
#include "Events/Event.h"
#include "Events/MouseEvent.h"

#include <glm/glm.hpp>

namespace BrokenSim
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(float fov, float aspectRatio, float nearClip , float farClip, float viewportWidth = 1280.0f, float viewportHeight = 720.0f);

		void SetProjection(float fov, float aspectRatio, float nearClip, float farClip);
		void SetViewportSize(float width, float height);

		void SetPosition(const glm::vec3& position);
		void SetRotation(float yaw, float pitch);

		void OnUpdate(TimeStep ts);
		void OnEvent(Event& e);

		// 获取摄像机矩阵
		glm::mat4 GetProjectionMatrix();
		const glm::mat4& GetProjectionMatrix() const;

		glm::mat4 GetViewMatrix();
		const glm::mat4& GetViewMatrix() const;

		const glm::mat4 GetViewProjectionMatrix() const;

		// 摄像机参数
		glm::vec3& GetPosition();
		const glm::vec3& GetPosition() const;

		glm::vec3 GetUp() const;
		glm::vec3 GetForward() const;
		glm::vec3 GetRight() const;
		glm::quat GetOrientation() const;

		// 摄像机投影参数
		float GetFOV() const;
		float GetAspectRatio() const;
		float GetNear() const;
		float GetFar() const;

		float GetViewportWidth() const;
		float GetViewportHeight() const;

		float GetYaw() const;
		float GetPitch() const;

	private:
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
		glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);

		// 摄像机位置
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };

		//摄像机投影参数
		float m_FOV = 45.0f;
		float m_AspectRatio = 16.0f / 9.0f;
		float m_Near = 0.1f;
		float m_Far = 1000.0f;

		// 视口参数
		float m_ViewportWidth = 1280.0f;
		float m_ViewportHeight = 720.0f;

		// 欧拉角
		float m_Yaw = -90.0f;
		float m_Pitch = 0.0f;

		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };
		float m_Distance = 10.0f;
		glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };

		// 鼠标灵敏度
		float m_MouseSensitivity = 0.1f;

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);

		void UpdateProjection();
		void UpdateView();

		glm::vec3 CalculatePosition();

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;

		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		void RecalculateViewMatrix();
	};
}
