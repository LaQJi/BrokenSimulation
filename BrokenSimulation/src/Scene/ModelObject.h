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
		ModelObject(unsigned int id, const std::string& path, Object* parent = nullptr, const std::string& name = "Model");
		virtual ~ModelObject();

		void Bind() const;
		void Unbind() const;

		virtual void OnUpdate(TimeStep ts) override;

		virtual void OnRender() override;

		virtual void OnEvent(Event& e) override;

	private:
		std::vector<std::shared_ptr<VertexArray>> m_VertexArrays;
		std::shared_ptr<Shader> m_Shader;

		std::string m_Name;
		std::string m_Path;

		bool LoadModel(const std::string& path);
		void ProcessNode(aiNode* node, const aiScene* scene);
	};
}