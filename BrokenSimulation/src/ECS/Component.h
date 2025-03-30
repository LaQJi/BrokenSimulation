#pragma once

#include "Core/TimeStep.h"

#include <imgui/imgui.h>
#include <glm/glm.hpp>

namespace BrokenSim
{
	class Entity;

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

		// 获取变换矩阵
		const glm::mat4& GetTransform() const;

		// 获取位置
		glm::vec3& GetPosition();
		
		// 获取旋转
		glm::vec3& GetRotation();

		// 获取缩放
		glm::vec3& GetScale();

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

		// 位置
		glm::vec3 m_Position = { 0.0f,0.0f,0.0f };
		// 旋转
		glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };
		// 缩放
		glm::vec3 m_Scale = { 1.0f, 1.0f, 1.0f };

		friend class Entity;
	};
}
