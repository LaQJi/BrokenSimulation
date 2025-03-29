#include "bspch.h"
#include "ECS/Entity.h"

#include <glm/gtc/matrix_transform.hpp>

namespace BrokenSim
{
	Entity::Entity(unsigned int id, Scene* scene, const std::string& name)
		: m_ID(id),
		m_Scene(scene),
		m_Name(name)
	{
	}

	Entity::~Entity()
	{
		RemoveFromParent();
		RemoveAllChildren();
	}

	void Entity::OnUpdate(TimeStep ts, Shader* shader)
	{

	}

	void Entity::OnRender(Shader* shader)
	{
	}

	void Entity::SetParent(Entity* parent)
	{
		if (parent == this || IsAncestor(parent))
		{
			BS_CORE_WARN("Entity::SetParent: parent is self");
			return;
		}
		if (m_Parent)
		{
			m_Parent->RemoveChild(this);
		}

		m_Parent = parent;
		if (m_Parent)
		{
			m_Parent->m_Children.push_back(this);
		}
	}

	Entity* Entity::GetRoot()
	{
		Entity* root = this;
		while (root->m_Parent)
		{
			root = root->m_Parent;
		}
		return root;
	}

	void Entity::AddChild(Entity* child)
	{
		if (child)
		{
			child->SetParent(this);
		}
		else
		{
			BS_CORE_WARN("Entity::AddChild: child is nullptr");
			return;
		}
	}

	void Entity::RemoveChild(Entity* child)
	{
		auto it = std::find(m_Children.begin(), m_Children.end(), child);
		if (it != m_Children.end())
		{
			(*it)->m_Parent = nullptr;
			m_Children.erase(it);
		}
		else
		{
			BS_CORE_WARN("Entity::RemoveChild: child not found");
		}
	}

	void Entity::RemoveFromParent()
	{
		if (m_Parent)
		{
			m_Parent->RemoveChild(this);
		}
	}

	void Entity::RemoveAllChildren()
	{
		for (auto& child : m_Children)
		{
			child->m_Parent = nullptr;
		}
		m_Children.clear();
	}

	const glm::mat4 Entity::GetModelMatrix() const
	{
		// ����ģ�;���
		glm::mat4 model = m_Transform;

		model = glm::translate(glm::mat4(1.0f), m_Position) * model;

		model = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) * model;
		model = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) * model;
		model = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) * model;

		model = glm::scale(glm::mat4(1.0f), m_Scale) * model;

		return model;
	}

	void Entity::ApplyTranslation()
	{
		m_Transform = glm::translate(glm::mat4(1.0f), m_Position) * m_Transform;

		m_Position = glm::vec3(0.0f);
	}

	void Entity::ApplyRotation()
	{
		m_Transform = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) * m_Transform;
		m_Transform = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) * m_Transform;
		m_Transform = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) * m_Transform;

		m_Rotation = glm::vec3(0.0f);
	}

	void Entity::ApplyScale()
	{
		m_Transform = glm::scale(glm::mat4(1.0f), m_Scale) * m_Transform;

		m_Scale = glm::vec3(1.0f);
	}

	bool Entity::IsAncestor(Entity* entity) const
	{
		if (!entity)
		{
			BS_CORE_WARN("Entity::IsAncestor: entity is nullptr");
			return false;
		}

		Entity* parent = m_Parent;
		while (parent)
		{
			if (parent == entity)
			{
				return true;
			}
			parent = parent->m_Parent;
		}
		return false;
	}
}