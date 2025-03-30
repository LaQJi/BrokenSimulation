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
		glm::vec4& GetColor() { return m_Color; }
		// 获取模型光照参数
		float& GetShininess() { return m_Shininess; }
		// 获取环境光强度
		float& GetAmbientStrength() { return m_AmbientStrength; }
		// 获取漫反射强度
		float& GetDiffuseStrength() { return m_DiffuseStrength; }
		// 获取镜面反射强度
		float& GetSpecularStrength() { return m_SpecularStrength; }


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
	};
}
