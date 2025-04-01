#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <typeindex>

#include "Core/Log.h"
#include "ECS/Component.h"
#include "ECS/SceneCamera.h"
#include "Renderer/Shader.h"

#include <glm/glm.hpp>

namespace BrokenSim
{
	class Scene;

	class Entity
	{
	public:
		Entity() = default;
		Entity(unsigned int id, Scene* scene, const std::string& name = "Entity");

		~Entity();

		void OnUpdate(TimeStep ts, Shader* shader);
		void OnRender(Shader* shader);
		void OnImGuiRender();


		// ��ȡΨһ��ʶ��
		unsigned int GetID() const;

		// ��ȡ����
		const std::string& GetName() const;
		// ��������
		void SetName(const std::string& name);

		// ��ȡ������
		Entity* GetParent() const;
		// ���ø�����
		void SetParent(Entity* parent);

		// ��ȡ������
		Entity* GetRoot();

		// ��ȡ�Ӷ���
		const std::vector<Entity*>& GetChildren() const;
		// ����Ӷ���
		void AddChild(Entity* child);
		// �Ƴ��Ӷ���
		void RemoveChild(Entity* child);
		// �Ӹ��ڵ㰲ȫ�Ƴ��Ӷ���
		void RemoveFromParent();
		// �Ƴ������Ӷ���
		void RemoveAllChildren();

		// ��ȡ�ɼ���
		bool GetVisible() const;
		// ���ÿɼ���
		void SetVisible(bool visible);

		// ��ȡ������
		bool GetEnabled() const;
		// ����������
		void SetEnabled(bool enabled);

		// ��ȡ�任����
		const glm::mat4& GetTransform() const;

		// ��ȡλ��
		glm::vec3& GetPosition();
		const glm::vec3& GetPosition() const;
		// ����λ��
		void SetPosition(const glm::vec3& position);

		// ��ȡ��ת
		glm::vec3& GetRotation();
		const glm::vec3& GetRotation() const;
		// ������ת
		void SetRotation(const glm::vec3& rotation);

		// ��ȡ����
		glm::vec3& GetScale();
		const glm::vec3& GetScale() const;
		// ��������
		void SetScale(const glm::vec3& scale);

		// ��ȡ����
		Scene* GetScene() const;

		// ��ȡģ�;���
		const glm::mat4 GetModelMatrix() const;

		// ������
		template<typename T, typename... Args>
		T* AddComponent(Args&&... args)
		{
			static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
			std::type_index type = std::type_index(typeid(T));
			// ����Ѿ����ڸ����͵�������򷵻�nullptr
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

		// �������
		template<typename T>
		bool HasComponent()
		{
			static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
			std::type_index type = std::type_index(typeid(T));
			return m_Components.find(type) != m_Components.end();
		}

		// ��ȡ���
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

		// �Ƴ��������������Ȩ
		template<typename T>
		std::unique_ptr<T> RemoveComponent()
		{
			static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
			std::type_index type = std::type_index(typeid(T));
			if (m_Components.find(type) != m_Components.end())
			{
				std::unique_ptr<T> removedComponent = std::unique_ptr<T>(static_cast<T*>(m_Components[type].release()));
				m_Components.erase(type);
				return removedComponent;
			}
			return nullptr;
		}

		// Ӧ��λ��
		void ApplyTranslation();

		// Ӧ����ת
		void ApplyRotation();

		// Ӧ������
		void ApplyScale();


	private:
		// ����Ƿ�Ϊ���ȶ��󣨷�ֹѭ�����ã�
		bool IsAncestor(Entity* entity) const;


	private:
		// Ψһ��ʶ��
		unsigned int m_ID;
		// ����
		std::string m_Name = "Entity";

		// ������
		Entity* m_Parent = nullptr;
		// �Ӷ���
		std::vector<Entity*> m_Children;

		// �Ƿ�ɼ�
		bool m_Visible = true;
		// �Ƿ�����
		bool m_Enabled = true;

		// �任����
		glm::mat4 m_Transform = glm::mat4(1.0f);
		// λ��
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		// ��ת
		glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };
		// ����
		glm::vec3 m_Scale = { 1.0f, 1.0f, 1.0f };

		// ����
		Scene* m_Scene = nullptr;
		// ����洢
		std::unordered_map<std::type_index, std::unique_ptr<Component>> m_Components;
	};
}
