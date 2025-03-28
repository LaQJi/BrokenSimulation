#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <typeindex>

#include "Core/Log.h"
#include "ECS/Component.h"
#include "ECS/Scene.h"

#include <glm/glm.hpp>

namespace BrokenSim
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(unsigned int id, Scene* scene, const std::string& name = "Entity");

		~Entity();

		// 获取唯一标识符
		unsigned int GetID() const { return m_ID; }

		// 获取名称
		const std::string& GetName() const { return m_Name; }
		// 设置名称
		void SetName(const std::string& name) { m_Name = name; }

		// 获取父对象
		Entity* GetParent() const { return m_Parent; }
		// 设置父对象
		void SetParent(Entity* parent);

		// 获取根对象
		Entity* GetRoot();

		// 获取子对象
		const std::vector<Entity*>& GetChildren() const { return m_Children; }
		// 添加子对象
		void AddChild(Entity* child);
		// 移除子对象
		void RemoveChild(Entity* child);
		// 从父节点安全移除子对象
		void RemoveFromParent();
		// 移除所有子对象
		void RemoveAllChildren();

		// 获取可见性
		bool GetVisible() const { return m_Visible; }
		// 设置可见性
		void SetVisible(bool visible) { m_Visible = visible; }

		// 获取启用性
		bool GetEnabled() const { return m_Enabled; }
		// 设置启用性
		void SetEnabled(bool enabled) { m_Enabled = enabled; }

		// 获取变换矩阵
		const glm::mat4& GetTransform() const { return m_Transform; }

		// 获取位置
		const glm::vec3& GetPosition() const { return m_Position; }
		// 设置位置
		void SetPosition(const glm::vec3& position) { m_Position = position; }

		// 获取旋转
		const glm::vec3& GetRotation() const { return m_Rotation; }
		// 设置旋转
		void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; }

		// 获取缩放
		const glm::vec3& GetScale() const { return m_Scale; }
		// 设置缩放
		void SetScale(const glm::vec3& scale) { m_Scale = scale; }

		// 获取场景
		Scene* GetScene() const { return m_Scene; }

		// 添加组件
		template<typename T, typename... Args>
		T* AddComponent(Args&&... args)
		{
			static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
			std::type_index type = std::type_index(typeid(T));
			// 如果已经存在该类型的组件，则返回nullptr
			if (HasComponent<T>())
			{
				BS_CORE_WARN("Entity already has component of type {0}", type.name());
				return nullptr;
			}
			auto component = std::make_unique<T>(this, std::forward<Args>(args)...);
			T* componentPtr = component.get();
			m_Components[type] = std::move(component);
			return componentPtr;
		}

		// 查找组件
		template<typename T>
		bool HasComponent()
		{
			static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
			std::type_index type = std::type_index(typeid(T));
			return m_Components.find(type) != m_Components.end();
		}

		// 获取组件
		template<typename T>
		T* GetComponent()
		{
			static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
			std::type_index type = std::type_index(typeid(T));
			if (m_Components.find(type) != m_Components.end())
			{
				return static_cast<T*>(m_Components[type].get());
			}
			return nullptr;
		}

		// 移除组件，返回所有权
		template<typename T>
		std::unique_ptr<T> RemoveComponent()
		{
			static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
			std::type_index type = std::type_index(typeid(T));
			if (m_Components.find(type) != m_Components.end())
			{
				std::unique_ptr<T> removedComponent = std::move(std::dynamic_pointer_cast<T>(m_Components[type]));
				m_Components.erase(type);
				return removedComponent;
			}
			return nullptr;
		}

		// 应用位移
		void ApplyTranslation();

		// 应用旋转
		void ApplyRotation();

		// 应用缩放
		void ApplyScale();


	private:
		// 检查是否为祖先对象（防止循环引用）
		bool IsAncestor(Entity* entity) const;


	private:
		// 唯一标识符
		unsigned int m_ID;
		// 名称
		std::string m_Name = "Entity";

		// 父对象
		Entity* m_Parent = nullptr;
		// 子对象
		std::vector<Entity*> m_Children;

		// 是否可见
		bool m_Visible = true;
		// 是否启用
		bool m_Enabled = true;

		// 变换矩阵
		glm::mat4 m_Transform = glm::mat4(1.0f);
		// 位置
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		// 旋转
		glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };
		// 缩放
		glm::vec3 m_Scale = { 1.0f, 1.0f, 1.0f };

		// 场景
		Scene* m_Scene = nullptr;
		// 组件存储
		std::unordered_map<std::type_index, std::unique_ptr<Component>> m_Components;
	};
}
