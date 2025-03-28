#pragma once

#include <queue>

#include "Core/TimeStep.h"
#include "ECS/Entity.h"
#include "ECS/SceneCamera.h"

namespace BrokenSim
{
	class Scene
	{
	public:
		virtual ~Scene() = default;

		virtual void OnUpdate(TimeStep ts) = 0;
		virtual void OnRender() = 0;
		virtual void OnImGuiRender() = 0;

		Entity* CreateEntity(const std::string& name = "Entity");

		void DestroyEntity(Entity* entity);

		Entity* FindEntity(unsigned int id);


	private:
		unsigned int AssignID();
		void FreeID(unsigned int id);


	private:
		// ���ڴ洢�����е�ʵ��
		std::vector<std::unique_ptr<Entity>> m_Entities;
		// ���ڸ���id���ٲ��ҳ����е�ʵ��
		std::unordered_map<unsigned int, Entity*> m_EntityMap;

		SceneCamera m_Camera;
		std::vector<LightComponent*> m_Lights;

		// ��ʵ��
		Entity m_RootEntity;

		// ������С��
		struct  compore
		{
			bool operator()(const unsigned int& a, const unsigned int& b) {
				return a > b;
			}
		};

		// ���ڴ洢���յ�ʵ��id
		std::priority_queue<unsigned int, std::vector<unsigned int>, compore> m_AvailableIDs;
		// ��һ�����õ�ʵ��id
		unsigned int m_NextID = 0;
	};
}
