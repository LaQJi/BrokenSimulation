#include "bspch.h"
#include "Scene/Scene.h"

namespace BrokenSim
{
	std::shared_ptr<Object> Scene::CreateObject(const Object::Type type, const std::string& name, Object* parent, const std::vector<std::any>& params)
	{
		if (auto iter = m_ObjectFactory.find(type); iter != m_ObjectFactory.end())
		{
			if (parent != nullptr)
			{
				unsigned int parentID = parent->GetID();

				if (m_Objects.find(parentID) == m_Objects.end())
				{
					BS_CORE_ERROR("Parent object not found!");
					return NullObject;
				}
				else if (m_Objects[parentID].get() != parent)
				{
					BS_CORE_ERROR("Parent object not found!");
					return NullObject;
				}
				else
				{
					unsigned int id = AssignID();
					std::string objectName;

					if (name == "")
					{
						objectName = GetTypeName(type);
					}
					else
					{
						objectName = name;
					}

					std::shared_ptr<Object> object = iter->second(id, objectName, parent, params);

					m_Objects[id] = object;
					parent->AddChild(object);

					return object;
				}
			}
			else
			{
				unsigned int id = AssignID();
				std::string objectName;

				if (name == "")
				{
					objectName = GetTypeName(type);
				}
				else
				{
					objectName = name;
				}

				std::shared_ptr<Object> object = iter->second(id, objectName, parent, params);

				m_Objects[id] = object;

				return object;
			}
		}
		else
		{
			BS_CORE_ERROR("Object type not found!");
			return NullObject;
		}
	}

	void Scene::DeleteObject(std::shared_ptr<Object>& object)
	{
		std::vector<unsigned int> ids;
		// 查找根Object是否在Scene中
		Object* root = object.get();
		while (root->GetParent() != nullptr)
		{
			root = root->GetParent();
		}
		unsigned int rootID = root->GetID();

		if (m_Objects.find(rootID) == m_Objects.end())
		{
			BS_CORE_ERROR("Object not found in Scene!");
			return;
		}
		else if (m_Objects[rootID].get() != root)
		{
			BS_CORE_ERROR("Object not found in Scene!");
			return;
		}
		else
		{
			// 删除Object及其所有子Object
			ids.push_back(rootID);

			if (object->GetChildren().size() > 0)
			{
				std::vector<unsigned int> childrenIDs = object->RemoveAllChildren();
			}

			ids.insert(ids.end(), object->RemoveAllChildren().begin(), object->RemoveAllChildren().end());

			for (auto& id : ids)
			{
				m_AvailableIDs.push(id);
			}

			// 若Object为根Object，则从Scene中删除
			if (object->GetParent() == nullptr)
			{
				m_Objects.erase(object->GetID());
			}
		}
	}


	bool Scene::IsObjectInScene(std::shared_ptr<Object>& object)
	{
		auto iter = m_Objects.find(object->GetID());
		if (iter == m_Objects.end())
		{
			BS_CORE_ERROR("Object not found in Scene!");
			return false;
		}
		else if (iter->second.get() != object.get())
		{
			BS_CORE_ERROR("Object not found in Scene!");
			return false;
		}
		else
		{
			return true;
		}
	}

	void Scene::Init(std::shared_ptr<Shader>& shader)
	{
		m_AvailableIDs.push(0);

		m_Shader = shader;
	}

	unsigned int Scene::AssignID()
	{
		// 若可用ID池为空，则创建新ID
		if (m_AvailableIDs.empty())
		{
			BS_CORE_WARN("Illegal operation in using ID pool, create a new ID!");
			unsigned int id = m_Objects.size();
			
			// 查找是否存在相同ID，若存在则自增
			auto iter = m_Objects.find(id);
			while (iter != m_Objects.end())
			{
				id++;
				iter = m_Objects.find(id);
			}

			return id;
		}
		else
		{
			unsigned int id = m_AvailableIDs.top();
			m_AvailableIDs.pop();

			// 维护可用ID池
			if (m_AvailableIDs.empty())
			{
				m_AvailableIDs.push(id + 1);
			}

			return id;
		}
	}

	void Scene::FreeID(unsigned int id)
	{
		m_AvailableIDs.push(id);
	}

	std::shared_ptr<Object>& Scene::GetObject(unsigned int id)
	{
		auto iter = m_Objects.find(id);
		if (iter == m_Objects.end())
		{
			BS_CORE_ERROR("Object not found in Scene!");
			return NullObject;
		}
		else
		{
			return iter->second;
		}
	}
}
