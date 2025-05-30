#pragma once

#include "Core/ResourceManager.h"
#include "ECS/VoronoiComponent.h"
#include "ECS/Scene.h"
#include "Renderer/VertexArray.h"

namespace BrokenSim
{
	class RenderSystem
	{
	public:
		explicit RenderSystem(std::shared_ptr<ResourceManager> resMgr);

		void InitVoronoiVertexArray();

		void SetClearColor(const glm::vec4& color);
		void Clear();

		void SetCurrentShader(const std::string& name);
		std::shared_ptr<Shader> GetCurrentShader() const;

		// 渲染场景
		void OnUpdate(TimeStep ts, Scene* scene);
		void OnWindowResize(unsigned int width, unsigned int height);

		// 渲染voronoi图
		void RenderVoronoi(TimeStep ts, VoronoiComponent* voronoiComponent);

		// 设置当前选中的实体
		void SetCurrentEntity(Entity* entity);
		Entity* GetCurrentEntity() const;

	private:
		// 渲染实体
		void RenderEntity(TimeStep ts, Entity* entity, glm::mat4 parentMatrix);

		void DrawVertexArray(VertexArray* va);

	private:
		std::shared_ptr<ResourceManager> m_ResourceManager;
		std::shared_ptr<Shader> m_CurrentShader = nullptr;

		// 当前选中的实体
		Entity* m_CurrentEntity = nullptr;

		unsigned int m_Width = 1280;
		unsigned int m_Height = 720;

		// 渲染voronoi图的顶点数组
		std::shared_ptr<VertexArray> m_VoronoiVertexArray;
		// 渲染voronoi图的着色器
		std::shared_ptr<Shader> m_VoronoiShader;
		// 渲染将voronoi图映射至实体的着色器
		std::shared_ptr<Shader> m_VoronoiMappingShader;
	};
}
