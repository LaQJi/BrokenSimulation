#pragma once

#include "Core/TimeStep.h"

#include <imgui/imgui.h>
#include <glm/glm.hpp>

namespace BrokenSim
{
	class Entity;

	// TODO����ECS�ܹ�������󣬺�����������Ʋ�ʵ��

	class Component
	{
	public:
		// ���ÿ����͸�ֵ
		Component(const Component&) = delete;
		Component& operator=(const Component&) = delete;
		// Ĭ����������
		virtual ~Component() = default;

		// ��ӵ�ʵ��ʱ����
		virtual void OnAttach() {}
		// ��ʵ���Ƴ�ʱ����
		virtual void OnDetach() {}
		// ÿ֡����ʱ����
		virtual void OnUpdate(TimeStep ts) {}

		Entity* GetOwner() { return m_Owner; }
		const std::string& GetName() const { return m_Name; }
		bool IsEnabled() const { return m_Enabled; }
		void SetEnabled(bool enabled) { m_Enabled = enabled; }

		// ��ȡ�任����
		const glm::mat4& GetTransform() const;

		// ��ȡλ��
		glm::vec3& GetPosition();
		// ����λ��
		void SetPosition(const glm::vec3& position);
		
		// ��ȡ��ת
		glm::vec3& GetRotation();
		// ������ת
		void SetRotation(const glm::vec3& rotation);

		// ��ȡ����
		glm::vec3& GetScale();
		// ��������
		void SetScale(const glm::vec3& scale);

	protected:
		// �������
		std::string m_Name;
		// �������ʵ��
		Entity* m_Owner;
		// ����Ƿ�����
		bool m_Enabled = true;
		explicit Component(Entity* owner)
			: m_Owner(owner)
		{
			m_Name = typeid(*this).name();
		}

		// λ��
		glm::vec3 m_Position = { 0.0f,0.0f,0.0f };
		// ��ת
		glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };
		// ����
		glm::vec3 m_Scale = { 1.0f, 1.0f, 1.0f };

		friend class Entity;
	};
}
