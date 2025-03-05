#pragma once

#include <fstream>
#include <sstream>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "util.h"
#include "Timer.h"

#include "glm/glm.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

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

	inline const IndexBuffer* GetIndexBuffer() const { return ib.get(); }

private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	std::unique_ptr<VertexArray> va;
	std::unique_ptr<IndexBuffer> ib;

	std::string directory;
	std::string name;

	void loadModel(const std::string& path);
	void ProcessNode(aiNode* node, const aiScene* scene);
};
