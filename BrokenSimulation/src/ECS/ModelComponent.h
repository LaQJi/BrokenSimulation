#pragma once

#include "ECS/Component.h"
#include "Renderer/VertexArray.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace BrokenSim
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;

		bool operator==(const Vertex& other) const
		{
			return Position == other.Position &&
				Normal == other.Normal &&
				TexCoords == other.TexCoords;
		}
	};

	struct Meshes
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
	};

	struct AABB
	{
		glm::vec3 minPoint;
		glm::vec3 maxPoint;

		glm::vec3 GetCenter() const
		{
			return (minPoint + maxPoint) * 0.5f;
		}
	};

	class ModelComponent : public Component
	{
	public:
		ModelComponent(Entity* owner, const std::string& path);
		ModelComponent(Entity* owner, const Meshes& meshes);

		~ModelComponent();

		void OnUpdate(TimeStep ts) override;

		void UpdateMeshes(const Meshes& meshes);
		
		VertexArray* GetVertexArray() { return m_VertexArray.get(); }
		
		// ��ȡģ����ɫ
		glm::vec4& GetColor();
		// ����ģ����ɫ
		void SetColor(const glm::vec4& color);

		// ��ȡģ�͹��ղ���
		float& GetShininess();
		// ����ģ�͹��ղ���
		void SetShininess(float shininess);
		
		// ��ȡ������ǿ��
		float& GetAmbientStrength();
		// ���û�����ǿ��
		void SetAmbientStrength(float strength);
		
		// ��ȡ������ǿ��
		float& GetDiffuseStrength();
		// ����������ǿ��
		void SetDiffuseStrength(float strength);
		
		// ��ȡ���淴��ǿ��
		float& GetSpecularStrength();
		// ���þ��淴��ǿ��
		void SetSpecularStrength(float strength);

		// ��ȡģ�ͼ�������
		const glm::vec3& GetGeometryCenter() const;

		// ��ȡģ��·��
		const std::string& GetPath() const;

		// ��ȡģ��y����Сֵ
		float GetYMin() const;

		// ��ȡģ��y�����ֵ
		float GetYMax() const;

		// ��ȡģ�Ͷ���
		const std::vector<Vertex>& GetVertices() const;

		// ��ȡģ������
		const std::vector<unsigned int>& GetIndices() const;

		// ��ȡģ�Ͱ�Χ��
		const AABB& GetBoundingBox() const;

	private:
		bool LoadModel(const std::string& path);
		void ProcessNode(aiNode* node, const aiScene* scene);


	private:
		// ģ�Ͷ�������
		std::unique_ptr<VertexArray> m_VertexArray;

		// ģ��·��
		std::string m_Path;

		// ģ����ɫ
		glm::vec4 m_Color = { 0.8f, 0.8f, 0.8f, 1.0f };

		// ģ�͹��ղ���
		float m_Shininess = 32.0f;
		// ������ǿ��
		float m_AmbientStrength = 0.3f;
		// ������ǿ��
		float m_DiffuseStrength = 0.8f;
		// ���淴��ǿ��
		float m_SpecularStrength = 1.0f;

		// ģ�Ͷ���
		std::vector<Vertex> m_Vertices;
		// ģ������
		std::vector<unsigned int> m_Indices;

		// ��������
		glm::vec3 m_GeometryCenter = { 0.0f, 0.0f, 0.0f };

		float yMin = FLT_MAX;
		float yMax = FLT_MIN;

		AABB m_BoudingBox;
	};
}
