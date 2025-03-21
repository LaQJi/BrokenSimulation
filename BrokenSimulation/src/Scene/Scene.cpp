#include "bspch.h"
#include "Scene/Scene.h"

#include "Core/Log.h"

namespace BrokenSim
{
	Scene::Scene()
	{
		// ��ʼ��ID
		m_AvailableIDs.push(0);

		// TODO: ע������Object�Ĺ�������
	}

	Scene::~Scene()
	{
	}

	void Scene::OnUpdate(TimeStep ts)
	{
		for (auto& object : m_Objects)
		{
			object.second->OnUpdate(ts);
		}
	}

	void Scene::OnRender()
	{
		for (auto& object : m_Objects)
		{
			object.second->OnRender();
		}
	}

	template <typename T>
	void Scene::RegisterFactoryFunction(const Object::Type& type)
	{
		m_FactoryFunctionMap[type] = [](int id, const std::string& name) {
			return std::make_shared<T>(id, name);
			};
	}

	template<typename T, typename ...Args>
	std::shared_ptr<T> Scene::CreateObject(const Object::Type& type, const std::string& name, Args&&... args)
	{
		if (m_FactoryFunctionMap.find(T) == m_FactoryFunctionMap.end())
		{
			BS_CORE_ERROR(false, "Factory function not registered!");
			return nullptr;
		}

		// ����ID
		unsigned int id = m_AvailableIDs.top();

		// �ӿ���ID���Ƴ�
		m_AvailableIDs.pop();

		// ά������ID
		if (m_AvailableIDs.empty())
		{
			m_AvailableIDs.push(id + 1);
		}

		// ����Object
		auto object = m_FactoryFunctionMap[type](id, name);
		m_Objects[id] = object;
		
		return std::dynamic_pointer_cast<T>(object);
	}

	template<typename T, typename ...Args>
	std::shared_ptr<T> Scene::CreateChildObject(const Object::Type& type, const std::string& name, EmptyObject* parent, Args && ...args)
	{
		// ���Ҹ�Object�Ƿ���Scene��
		Object* root = parent;
		while (root->GetParent() != nullptr)
		{
			root = root->GetParent();
		}
		unsigned int rootID = root->GetID();

		if (m_Objects.find(rootID) == m_Objects.end())
		{
			BS_CORE_ERROR(false, "Root Object not found in Scene!");
			return nullptr;
		}
		else if (m_Objects[rootID].get() != root)
		{
			BS_CORE_ERROR(false, "Root Object not found in Scene!");
			return nullptr;
		}

		if (m_FactoryFunctionMap.find(T) == m_FactoryFunctionMap.end())
		{
			BS_CORE_ERROR(false, "Factory function not registered!");
			return nullptr;
		}

		// ����ID
		unsigned int id = m_AvailableIDs.top();

		// �ӿ���ID���Ƴ�
		m_AvailableIDs.pop();

		// ά������ID
		if (m_AvailableIDs.empty())
		{
			m_AvailableIDs.push(id + 1);
		}

		// ����Object
		auto object = m_FactoryFunctionMap[type](id, name);
		parent->AddChild(object);

		return std::dynamic_pointer_cast<T>(object);
	}

	void Scene::DeleteObject(std::shared_ptr<Object> object)
	{
		std::vector<unsigned int> ids;
		// ���Ҹ�Object�Ƿ���Scene��
		Object* root = object.get();
		while (root != nullptr)
		{
			root = root->GetParent();
		}
		unsigned int rootID = root->GetID();

		if (m_Objects.find(rootID) == m_Objects.end())
		{
			BS_CORE_ERROR(false, "Object not found in Scene!");
			return;
		}
		else if (m_Objects[rootID].get() != root)
		{
			BS_CORE_ERROR(false, "Object not found in Scene!");
			return;
		}
		else
		{
			// ɾ��Object����������Object
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

			// ��ObjectΪ��Object�����Scene��ɾ��
			if (object->GetParent() == nullptr)
			{
				m_Objects.erase(id);
			}
		}
	}
}
