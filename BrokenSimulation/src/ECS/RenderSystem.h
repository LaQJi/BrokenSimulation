#pragma once

#include "Core/ResourceManager.h"
#include "ECS/VoronoiComponent.h"
#include "ECS/Entity.h"

namespace BrokenSim
{
	class RenderSystem
	{
	public:
		explicit RenderSystem(std::shared_ptr<ResourceManager> resMgr);

		void InitVoronoiVertexArray();

		void SetClearColor(const glm::vec4& color);
		void Clear();

		void SetShader(const std::string& name);
		std::shared_ptr<Shader> GetCurrentShader() const;

		// ��Ⱦ����
		void OnUpdate(TimeStep ts, Scene* scene);
		void OnWindowResize(unsigned int width, unsigned int height);

		// ��Ⱦvoronoiͼ
		void OnUpdate(TimeStep ts, VoronoiComponent* voronoiComponent);

	private:
		// ��Ⱦʵ��
		void RenderEntity(TimeStep ts, Entity* entity, glm::mat4 parentMatrix);

		void DrawVertexArray(VertexArray* va);

	private:
		std::shared_ptr<ResourceManager> m_ResourceManager;
		std::shared_ptr<Shader> m_CurrentShader = nullptr;

		unsigned int m_Width = 1280;
		unsigned int m_Height = 720;

		// ��Ⱦvoronoiͼ�Ķ�������
		std::shared_ptr<VertexArray> m_VoronoiVertexArray;
		// ��Ⱦvoronoiͼ����ɫ��
		std::shared_ptr<Shader> m_VoronoiShader;
	};
}
