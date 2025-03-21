#include "bspch.h"
#include "Scene/Object.h"

#include <glm/ext/matrix_transform.hpp>

namespace BrokenSim
{
	unsigned int Object::GetID() const
	{
		return id;
	}

	Object::Type Object::GetType() const
	{
		return type;
	}

	const std::string& Object::GetName() const
	{
		return m_Name;
	}

	const glm::mat4& Object::GetTransform() const
	{
		return m_Transform;
	}

	Object* Object::GetParent() const
	{
		return m_Parent;
	}

	const std::vector<std::shared_ptr<Object>>& Object::GetChildren() const
	{
		return m_Children;
	}

	const bool Object::GetVisible() const
	{
		return m_Visible;
	}

	const bool Object::GetEnabled() const
	{
		return m_Enabled;
	}

	const glm::vec3& Object::GetPosition() const
	{
		return m_Position;
	}

	const glm::vec3& Object::GetRotation() const
	{
		return m_Rotation;
	}

	const glm::vec3& Object::GetScale() const
	{
		return m_Scale;
	}

	const glm::mat4 Object::GetModelMatrix() const
	{
		// 计算模型矩阵
		glm::mat4 model = m_Transform;

		model = glm::translate(model, m_Position);

		model = glm::rotate(model, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		model = glm::scale(model, m_Scale);

		return model;
	}



	void Object::SetName(const std::string& name)
	{
		this->m_Name = name;
	}

	void Object::SetParent(Object* parent)
	{
		this->m_Parent = parent;
	}

	void Object::AddChild(std::shared_ptr<Object> child)
	{
		m_Children.push_back(child);

		if (child->GetParent() != this)
		{
			// 如果子对象已经有父对象，先从原父对象中移除
			if (child->GetParent())
			{
				child->GetParent()->RemoveChild(child);
			}
			child->SetParent(this);
		}
	}

	std::vector<unsigned int> Object::RemoveChild(std::shared_ptr<Object> child)
	{
		std::vector<unsigned int> removedIDs;
		auto it = std::find(m_Children.begin(), m_Children.end(), child);
		if (it != m_Children.end())
		{
			m_Children.erase(it);
			removedIDs.push_back(child->GetID());

			// 解除父子关系
			child->SetParent(nullptr);
			if (child->GetChildren().size() > 0)
			{
				std::vector<unsigned int> childIds =  child->RemoveAllChildren();
				removedIDs.insert(removedIDs.end(), childIds.begin(), childIds.end());
			}
		}
		else
			BS_CORE_WARN("Object::RemoveChild: child not found");
	}

	std::vector<unsigned int> Object::RemoveAllChildren()
	{
		std::vector<unsigned int> removedIDs;
		for (auto& child : m_Children)
		{
			removedIDs.push_back(child->GetID());
			child->SetParent(nullptr);
			if (child->GetChildren().size() > 0)
			{
				std::vector<unsigned int> childIds = child->RemoveAllChildren();
				removedIDs.insert(removedIDs.end(), childIds.begin(), childIds.end());
			}
		}
		m_Children.clear();
	}

	void Object::SetVisible(bool visible)
	{
		this->m_Visible = visible;
	}

	void Object::SetEnabled(bool enabled)
	{
		this->m_Enabled = enabled;
	}

	void Object::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
	}

	void Object::SetRotation(const glm::vec3& rotation)
	{
		m_Rotation = rotation;
	}

	void Object::SetScale(const glm::vec3& scale)
	{
		m_Scale = scale;
	}



	void Object::ApplyTranslation()
	{
		this->Translate(m_Position);

		m_Position = glm::vec3(0.0f);
	}

	void Object::ApplyRotation()
	{
		this->Rotate(glm::vec3(1.0f, 0.0f, 0.0f), m_Rotation.x);
		this->Rotate(glm::vec3(0.0f, 1.0f, 0.0f), m_Rotation.y);
		this->Rotate(glm::vec3(0.0f, 0.0f, 1.0f), m_Rotation.z);

		m_Rotation = glm::vec3(0.0f);
	}

	void Object::ApplyScale()
	{
		this->Scale(m_Scale);

		m_Scale = glm::vec3(1.0f);
	}



	void Object::Rotate(const glm::vec3& axis, float angle)
	{
		m_Transform = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis) * m_Transform;
	}

	void Object::Translate(const glm::vec3& offset)
	{
		m_Transform = glm::translate(glm::mat4(1.0f), offset) * m_Transform;
	}

	void Object::Scale(const glm::vec3& scale)
	{
		m_Transform = glm::scale(glm::mat4(1.0f), scale) * m_Transform;
	}
}
