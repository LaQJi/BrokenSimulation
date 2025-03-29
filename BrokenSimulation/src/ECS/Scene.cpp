#include "bspch.h"
#include "ECS/Scene.h"

namespace BrokenSim
{
	Scene::Scene()
	{
		// 设置根实体
		unsigned int id = AssignID();
		m_RootEntity = std::make_unique<Entity>(id, this, "Root");
		// 设置相机
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
		// 创建实体
		unsigned int id = AssignID();
		std::unique_ptr<Entity> entity = std::make_unique<Entity>(id, this, name);

		// 获取实体指针
		Entity* result = entity.get();

		if (!parent)
		{
			entity->SetParent(m_RootEntity.get());
		}
		else
		{
			entity->SetParent(parent);
		}

		// 将实体添加到实体列表
		m_Entities.push_back(std::move(entity));
		// 将实体添加到实体映射
		m_EntityMap[id] = result;

		return result;
	}

	void Scene::DestroyEntity(Entity* entity)
	{
		// 递归销毁实体的所有子实体
		for (auto* child : entity->GetChildren())
		{
			DestroyEntity(child);
		}

		// 从父实体中移除实体（如果有）
		if (entity->GetParent())
		{
			entity->GetParent()->RemoveChild(entity);
		}

		// 从实体映射中移除实体
		m_EntityMap.erase(entity->GetID());

		// 从实体列表中移除实体
		m_Entities.erase(std::remove_if(m_Entities.begin(), m_Entities.end(),
			[&](const std::unique_ptr<Entity>& e)
			{
				return e.get() == entity;
			}));

		// 释放实体ID
		FreeID(entity->GetID());
	}

	Entity* Scene::FindEntity(unsigned int id)
	{
		// 使用ID查找实体
		if (auto iter = m_EntityMap.find(id); iter != m_EntityMap.end())
		{
			// 返回实体
			return iter->second;
		}
		else
		{
			// 未找到实体
			return nullptr;
		}
	}

	std::vector<Entity*> Scene::GetEntities() const
	{
		return m_RootEntity->GetChildren();
	}

	unsigned int Scene::AssignID()
	{
		// 如果没有可用的ID，返回下一个ID
		if (m_AvailableIDs.empty())
		{
			return m_NextID++;
		}
		else
		{
			// 从队列中取出一个最小的可用ID
			unsigned int id = m_AvailableIDs.top();
			m_AvailableIDs.pop();
			return id;
		}
	}

	void Scene::FreeID(unsigned int id)
	{
		// 将ID放回队列
		m_AvailableIDs.push(id);
	}
}
