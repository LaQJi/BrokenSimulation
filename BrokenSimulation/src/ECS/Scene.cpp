#include "bspch.h"
#include "ECS/Scene.h"

namespace BrokenSim
{
	Scene::Scene()
	{
		// ���ø�ʵ��
		unsigned int id = AssignID();
		m_RootEntity = std::make_unique<Entity>(id, this, "Root");
		// �������
		m_Camera = std::make_unique<SceneCamera>();
	}

	Scene::~Scene()
	{
	}

	void Scene::OnUpdate(TimeStep ts)
	{
	}

	void Scene::OnRender()
	{
	}

	void Scene::OnImGuiRender()
	{
	}

	Entity* Scene::CreateEntity(const std::string& name, Entity* parent)
	{
		// ����ʵ��
		unsigned int id = AssignID();
		std::unique_ptr<Entity> entity = std::make_unique<Entity>(id, this, name);

		// ��ȡʵ��ָ��
		Entity* result = entity.get();

		if (!parent)
		{
			entity->SetParent(m_RootEntity.get());
		}
		else
		{
			entity->SetParent(parent);
		}

		// ��ʵ����ӵ�ʵ���б�
		m_Entities.push_back(std::move(entity));
		// ��ʵ����ӵ�ʵ��ӳ��
		m_EntityMap[id] = result;

		return result;
	}

	void Scene::DestroyEntity(Entity* entity)
	{
		// �ݹ�����ʵ���������ʵ��
		for (auto* child : entity->GetChildren())
		{
			DestroyEntity(child);
		}

		// �Ӹ�ʵ�����Ƴ�ʵ�壨����У�
		if (entity->GetParent())
		{
			entity->GetParent()->RemoveChild(entity);
		}

		// ��ʵ��ӳ�����Ƴ�ʵ��
		m_EntityMap.erase(entity->GetID());

		// ��ʵ���б����Ƴ�ʵ��
		m_Entities.erase(std::remove_if(m_Entities.begin(), m_Entities.end(),
			[&](const std::unique_ptr<Entity>& e)
			{
				return e.get() == entity;
			}));

		// �ͷ�ʵ��ID
		FreeID(entity->GetID());
	}

	Entity* Scene::FindEntity(unsigned int id)
	{
		// ʹ��ID����ʵ��
		if (auto iter = m_EntityMap.find(id); iter != m_EntityMap.end())
		{
			// ����ʵ��
			return iter->second;
		}
		else
		{
			// δ�ҵ�ʵ��
			return nullptr;
		}
	}

	std::vector<Entity*> Scene::GetEntities() const
	{
		return m_RootEntity->GetChildren();
	}

	unsigned int Scene::AssignID()
	{
		// ���û�п��õ�ID��������һ��ID
		if (m_AvailableIDs.empty())
		{
			return m_NextID++;
		}
		else
		{
			// �Ӷ�����ȡ��һ����С�Ŀ���ID
			unsigned int id = m_AvailableIDs.top();
			m_AvailableIDs.pop();
			return id;
		}
	}

	void Scene::FreeID(unsigned int id)
	{
		// ��ID�Żض���
		m_AvailableIDs.push(id);
	}
}
