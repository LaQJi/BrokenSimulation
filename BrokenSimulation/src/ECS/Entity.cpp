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

	void Entity::OnImGuiRender()
	{
	}

	unsigned int Entity::GetID() const
	{
		return m_ID;
	}

	const std::string& Entity::GetName() const
	{
		return m_Name;
	}

	void Entity::SetName(const std::string& name)
	{
		m_Name = name;
	}

	Entity* Entity::GetParent() const
	{
		return m_Parent;
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

	const std::vector<Entity*>& Entity::GetChildren() const
	{
		return m_Children;
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

	bool Entity::GetVisible() const
	{
		return m_Visible;
	}

	void Entity::SetVisible(bool visible)
	{
		m_Visible = visible;
	}

	bool Entity::GetEnabled() const
	{
		return m_Enabled;
	}

	void Entity::SetEnabled(bool enabled)
	{
		m_Enabled = enabled;
	}

	const glm::mat4& Entity::GetTransform() const
	{
		return m_Transform;
	}

	glm::vec3& Entity::GetPosition()
	{
		return m_Position;
	}

	const glm::vec3& Entity::GetPosition() const
	{
		return m_Position;
	}

	void Entity::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
	}

	glm::vec3& Entity::GetRotation()
	{
		return m_Rotation;
	}

	const glm::vec3& Entity::GetRotation() const
	{
		return m_Rotation;
	}

	void Entity::SetRotation(const glm::vec3& rotation)
	{
		m_Rotation = rotation;
	}

	glm::vec3& Entity::GetScale()
	{
		return m_Scale;
	}

	const glm::vec3& Entity::GetScale() const
	{
		return m_Scale;
	}

	void Entity::SetScale(const glm::vec3& scale)
	{
		m_Scale = scale;
	}

	Scene* Entity::GetScene() const
	{
		return m_Scene;
	}

	const glm::mat4 Entity::GetModelMatrix() const
	{
		// ¼ÆËãÄ£ÐÍ¾ØÕó
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