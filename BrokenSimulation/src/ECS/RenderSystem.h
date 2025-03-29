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

		// 渲染场景
		void OnUpdate(TimeStep ts, Scene* scene);
		void OnWindowResize(unsigned int width, unsigned int height);

		// 渲染voronoi图
		void OnUpdate(TimeStep ts, VoronoiComponent* voronoiComponent);

	private:
		// 渲染实体
		void RenderEntity(TimeStep ts, Entity* entity, glm::mat4 parentMatrix);

		void DrawVertexArray(VertexArray* va);

	private:
		std::shared_ptr<ResourceManager> m_ResourceManager;
		std::shared_ptr<Shader> m_CurrentShader = nullptr;

		unsigned int m_Width = 1280;
		unsigned int m_Height = 720;

		// 渲染voronoi图的顶点数组
		std::shared_ptr<VertexArray> m_VoronoiVertexArray;
		// 渲染voronoi图的着色器
		std::shared_ptr<Shader> m_VoronoiShader;
	};
}
