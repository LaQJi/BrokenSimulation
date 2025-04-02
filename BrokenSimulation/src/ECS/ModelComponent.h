#pragma once

#include "ECS/Component.h"
#include "Renderer/VertexArray.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace BrokenSim
{
	class ModelComponent : public Component
	{
	public:
		ModelComponent(Entity* owner, const std::string& path);

		~ModelComponent();

		void OnUpdate(TimeStep ts) override;
		
		VertexArray* GetVertexArray() { return m_VertexArray.get(); }
		
		// 获取模型颜色
		glm::vec4& GetColor();
		// 设置模型颜色
		void SetColor(const glm::vec4& color);

		// 获取模型光照参数
		float& GetShininess();
		// 设置模型光照参数
		void SetShininess(float shininess);
		
		// 获取环境光强度
		float& GetAmbientStrength();
		// 设置环境光强度
		void SetAmbientStrength(float strength);
		
		// 获取漫反射强度
		float& GetDiffuseStrength();
		// 设置漫反射强度
		void SetDiffuseStrength(float strength);
		
		// 获取镜面反射强度
		float& GetSpecularStrength();
		// 设置镜面反射强度
		void SetSpecularStrength(float strength);

		// 获取模型几何中心
		const glm::vec3& GetGeometryCenter() const;

		// 获取模型路径
		const std::string& GetPath() const;

		// 获取模型y轴最小值
		float GetYMin() const;

		// 获取模型y轴最大值
		float GetYMax() const;

	private:
		bool LoadModel(const std::string& path);
		void ProcessNode(aiNode* node, const aiScene* scene);


	private:
		// 模型顶点数组
		std::unique_ptr<VertexArray> m_VertexArray;

		// 模型路径
		std::string m_Path;

		// 模型颜色
		glm::vec4 m_Color = { 0.8f, 0.8f, 0.8f, 1.0f };

		// 模型光照参数
		float m_Shininess = 32.0f;
		// 环境光强度
		float m_AmbientStrength = 0.3f;
		// 漫反射强度
		float m_DiffuseStrength = 0.8f;
		// 镜面反射强度
		float m_SpecularStrength = 1.0f;

		struct Vertex
		{
			glm::vec3 Position;
			glm::vec3 Normal;
			glm::vec2 TexCoords;
		};

		// 模型顶点
		std::vector<Vertex> m_Vertices;
		// 模型索引
		std::vector<unsigned int> m_Indices;

		// 几何中心
		glm::vec3 m_GeometryCenter = { 0.0f, 0.0f, 0.0f };

		float yMin = FLT_MAX;
		float yMax = FLT_MIN;
	};
}
