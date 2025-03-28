#pragma once

#include "Core/TimeStep.h"
#include "ECS/Entity.h"

namespace BrokenSim
{
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

		friend class Entity;
	};
}
