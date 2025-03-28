#pragma once

#include "ECS/Component.h"

namespace BrokenSim
{
	class CameraComponent : public Component
	{
	public:
		CameraComponent(Entity* owner,							// ӵ����
			float fov = 45.0f,									// ��Ұ�Ƕ�
			float aspectRatio = 16.0f / 9.0f,					// ��߱�
			float nearClip = 0.01f,								// ���ü���
			float farClip = 1000.0f);							// Զ�ü���

		// ��ȡͶӰ����
		glm::mat4 GetProjectionMatrix() const;

		// ��ȡ��ͼ����
		glm::mat4 GetViewMatrix() const;

		// ������Ұ�Ƕ�
		void SetFOV(float fov);
		// ��ȡ��Ұ�Ƕ�
		float GetFOV() const;

		// ���ÿ�߱�
		void SetAspectRatio(float aspectRatio);
		// ��ȡ��߱�
		float GetAspectRatio() const;

		// ���ý��ü���
		void SetNearClip(float nearClip);
		// ��ȡ���ü���
		float GetNearClip() const;

		// ����Զ�ü���
		void SetFarClip(float farClip);
		// ��ȡԶ�ü���
		float GetFarClip() const;

		// ����λ��
		void SetPosition(const glm::vec3& position);
		// ��ȡλ��
		glm::vec3 GetPosition() const;

		// ������ת
		void SetRotation(const glm::vec3& rotation);
		// ��ȡ��ת
		glm::vec3 GetRotation() const;
	
	private:
		// ��Ұ�Ƕ�
		float m_FOV;
		// ��߱�
		float m_AspectRatio;
		// ���ü���
		float m_NearClip;
		// Զ�ü���
		float m_FarClip;

		// λ��
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		// ��ת
		glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };
	};
}
