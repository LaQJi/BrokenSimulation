#pragma once

#include <queue>

#include "Core/TimeStep.h"
#include "ECS/Entity.h"
#include "ECS/SceneCamera.h"
#include "ECS/LightComponent.h"

namespace BrokenSim
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		void OnUpdate(TimeStep ts);
		void OnRender();
		void OnImGuiRender();

		Entity* CreateEntity(const std::string& name = "Entity", Entity* parent = nullptr);

		void DestroyEntity(Entity* entity);

		Entity* FindEntity(unsigned int id);

		const Entity* GetRootEntity() const { return m_RootEntity.get(); }
		std::vector<Entity*> GetEntities() const;

		SceneCamera* GetCamera() { return m_Camera.get(); }

		std::vector<LightComponent*> GetLights() { return m_Lights; }
		void AddLight(LightComponent* light) { m_Lights.push_back(light); }


	private:
		unsigned int AssignID();
		void FreeID(unsigned int id);


	private:
		// 用于存储场景中的实体
		std::vector<std::unique_ptr<Entity>> m_Entities;
		// 用于根据id快速查找场景中的实体
		std::unordered_map<unsigned int, Entity*> m_EntityMap;

		std::unique_ptr<SceneCamera> m_Camera;
		std::vector<LightComponent*> m_Lights;

		// 根实体
		std::unique_ptr<Entity> m_RootEntity;

		// 定义最小堆
		struct  compore
		{
			bool operator()(const unsigned int& a, const unsigned int& b) {
				return a > b;
			}
		};

		// 用于存储回收的实体id
		std::priority_queue<unsigned int, std::vector<unsigned int>, compore> m_AvailableIDs;
		// 下一个可用的实体id
		unsigned int m_NextID = 0;
	};
}
