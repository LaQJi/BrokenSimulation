#pragma once

#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/VertexBufferLayout.h"
#include "Renderer/IndexBuffer.h"
#include "Utils/util.h"
#include "Core/Timer.h"

#include "glm/glm.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace BrokenSim
{
	struct Vertex
	{
		// 顶点坐标
		glm::vec3 position;

		// 顶点法线
		glm::vec3 normal;
	};

	class Model
	{
	public:
		Model(const std::string& path);
		~Model();

		void Bind() const;
		void Unbind() const;

		const unsigned int GetIndexCount() const { return indices.size(); }

	private:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		std::unique_ptr<VertexArray> va;
		std::unique_ptr<VertexBuffer> vb;

		std::unique_ptr<IndexBuffer> ib;
		std::unique_ptr<VertexBufferLayout> layout;

		std::string directory;
		std::string name;

		bool loadModel(const std::string& path);
		void ProcessNode(aiNode* node, const aiScene* scene);
	};
}
