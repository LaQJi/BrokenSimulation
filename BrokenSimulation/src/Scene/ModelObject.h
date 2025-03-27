#pragma once

#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/VertexBufferLayout.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Shader.h"
#include "Scene/Object.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace BrokenSim
{
	class ModelObject : public Object
	{
	public:
		ModelObject(unsigned int id, const std::string& name = "Model", Object* parent = nullptr, const std::string path = "");
		virtual ~ModelObject();

		virtual void OnUpdate(TimeStep ts, std::shared_ptr<Shader>& shader) override;

		virtual void OnRender(std::shared_ptr<Shader> shader) override;

		virtual void OnEvent(Event& e) override;

		std::shared_ptr<VertexArray>& GetVertexArray() { return m_VertexArray; }

		glm::vec4& GetColor() { return m_Color; }

	private:
		std::shared_ptr<VertexArray> m_VertexArray;

		glm::vec4 m_Color = { 0.6f, 0.6f, 0.6f, 1.0f };

		float m_Shininess = 32.0f;

		float m_AmbientStrength = 0.3f;
		float m_DiffuseStrength = 0.7f;
		float m_SpecularStrength = 0.8f;

		struct Vertex
		{
			glm::vec3 Position;
			glm::vec3 Normal;
			glm::vec2 TexCoords;
		};

		std::vector<Vertex> m_Vertices;
		std::vector<unsigned int> m_Indices;

		std::string m_Path;

		bool LoadModel(const std::string& path);
		void ProcessNode(aiNode* node, const aiScene* scene);
	};
}