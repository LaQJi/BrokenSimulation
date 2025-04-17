#include "bspch.h"
#include "ECS/Scene.h"

#include "Geometry/ModelCut.h"
#include "ECS/ModelComponent.h"
#include "ECS/VoronoiComponent.h"

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
		// ��Ⱦ�����е�����ʵ��
		for (Entity* entity : GetEntities())
		{
			entity->OnImGuiRender();
		}
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

	void Scene::ApplyBreaking(Entity* entity)
	{
		ModelComponent* mc = entity->GetComponent<ModelComponent>();
		VoronoiComponent* vc = entity->GetComponent<VoronoiComponent>();

		std::vector<Meshes> meshes = Geometry::cutModel(*mc, *vc);

		Entity* parent = entity->GetParent();
		
		mc->UpdateMeshes(meshes[0]);

		for (int i = 1; i < meshes.size(); i++)
		{
			Entity* newEntity = CreateEntity(entity->GetName() + "_" + std::to_string(i), parent);
			newEntity->AddComponent<ModelComponent>(meshes[i]);
			newEntity->SetPosition(entity->GetPosition());
			newEntity->SetRotation(entity->GetRotation());
			newEntity->SetScale(entity->GetScale());
		}

		entity->RemoveComponent<VoronoiComponent>();
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
