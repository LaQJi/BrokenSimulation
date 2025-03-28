#pragma once

#include "ECS/Component.h"

namespace BrokenSim
{
	class CameraComponent : public Component
	{
	public:
		enum class ProjectionType
		{
			Perspective = 0,
			Orthographic = 1
		};
		CameraComponent(Entity* owner,							// ӵ����
			ProjectionType type = ProjectionType::Perspective,	// ͶӰ����
			float initialValue = 45.0f,							// ��ʼֵ
			float aspectRatio = 16.0f / 9.0f,					// ��߱�
			float nearClip = 0.01f,								// ���ü���
			float farClip = 1000.0f);							// Զ�ü���
	
	private:
		// ���������
		ProjectionType m_ProjectionType;	// ͶӰ����
		float m_FOV;						// ��Ұ�Ƕ�
		float m_OrthoSize;					// ������С
		float m_AspectRatio;				// ��߱�
		float m_NearClip;					// ���ü���
		float m_FarClip;					// Զ�ü���

		float m_Yaw;						// ƫ����
		float m_Pitch;						// ������
		float m_Roll;						// ������

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
