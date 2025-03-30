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
		// ��ȡģ����ɫ
		glm::vec4& GetColor() { return m_Color; }
		// ��ȡģ�͹��ղ���
		float& GetShininess() { return m_Shininess; }
		// ��ȡ������ǿ��
		float& GetAmbientStrength() { return m_AmbientStrength; }
		// ��ȡ������ǿ��
		float& GetDiffuseStrength() { return m_DiffuseStrength; }
		// ��ȡ���淴��ǿ��
		float& GetSpecularStrength() { return m_SpecularStrength; }


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

		struct Vertex
		{
			glm::vec3 Position;
			glm::vec3 Normal;
			glm::vec2 TexCoords;
		};

		// ģ�Ͷ���
		std::vector<Vertex> m_Vertices;
		// ģ������
		std::vector<unsigned int> m_Indices;
	};
}
