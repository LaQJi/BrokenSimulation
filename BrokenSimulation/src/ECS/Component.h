#pragma once

#include "Core/TimeStep.h"
#include "ECS/Entity.h"

namespace BrokenSim
{
	class Component
	{
	public:
		// 禁用拷贝和赋值
		Component(const Component&) = delete;
		Component& operator=(const Component&) = delete;
		// 默认析构函数
		virtual ~Component() = default;

		// 添加到实体时调用
		virtual void OnAttach() {}
		// 从实体移除时调用
		virtual void OnDetach() {}
		// 每帧更新时调用
		virtual void OnUpdate(TimeStep ts) {}

		Entity* GetOwner() { return m_Owner; }
		const std::string& GetName() const { return m_Name; }
		bool IsEnabled() const { return m_Enabled; }
		void SetEnabled(bool enabled) { m_Enabled = enabled; }

	protected:
		// 组件名称
		std::string m_Name;
		// 组件所属实体
		Entity* m_Owner;
		// 组件是否启用
		bool m_Enabled = true;
		explicit Component(Entity* owner)
			: m_Owner(owner)
		{
			m_Name = typeid(*this).name();
		}

		friend class Entity;
	};
}
