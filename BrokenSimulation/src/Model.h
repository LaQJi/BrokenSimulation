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
	// ��������
	glm::vec3 position;

	// ���㷨��
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
