#pragma once

#include "Core/TimeStep.h"
#include "Events/MouseEvent.h"

#include <glm/glm.hpp>

namespace BrokenSim
{
	class SceneCamera
	{
	public:
		enum class ProjectionType
		{
			Perspective = 0,	// ͸��
			Orthographic = 1	// ����
		};
		SceneCamera(ProjectionType type = ProjectionType::Perspective,			// ͶӰ����
			float initialValue = 45.0f,											// ��ʼֵ
			float aspectRatio = 16.0f / 9.0f,									// ��߱�
			float nearClip = 0.01f,												// ���ü���
			float farClip = 1000.0f);											// Զ�ü���

		void OnUpdate(TimeStep ts);
		void OnEvent(Event& e);

		// �л�ͶӰ����
		void SwitchProjectionType();

		// �����ӿڴ�С
		void SetViewportSize(float width, float height);

		// ����ͶӰ����
		void SetProjectionMatrix(float typeValue, float aspectRatio, float nearClip, float farClip);
		// ��ȡͶӰ����
		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		// ��ȡ��ͼ����
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		// ��ȡ��ͼͶӰ����
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

		// ��ȡ������Ϣ
		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		glm::quat GetOrientation() const;


	private:
		bool OnMouseScroll(MouseScrolledEvent& e);

		void UpdateProjection();
		void UpdateView();

		glm::vec3 CalculatePosition();

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;

		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		void UpdateAlternateValue();


	private:
		// ͶӰ����
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
		// ��ͼ����
		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
		// ��ͼͶӰ����
		glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);

		// ���������
		ProjectionType m_ProjectionType;	// ͶӰ����
		float m_FOV;						// ��Ұ�Ƕȣ�͸�ӣ�
		float m_OrthoSize;					// ������С��������
		float m_AspectRatio;				// ��߱�
		float m_NearClip;					// ���ü���
		float m_FarClip;					// Զ�ü���

		// ŷ����
		float m_Yaw = 0.0f;					// ƫ����
		float m_Pitch = 0.0f;				// ������
		float m_Roll = 0.0f;				// ��ת��

		// �����λ��
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };

		// �ӿڴ�С
		float m_ViewportWidth = 1280.0f;
		float m_ViewportHeight = 720.0f;

		// ����
		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };
		// ����
		float m_Distance = 10.0f;
		// ��ʼ���λ��
		glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };

		// ���������
		float m_MouseSensitivity = 0.1f;
	};
}