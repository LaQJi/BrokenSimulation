#include "Model.h"

Model::Model(const std::string& path)
{
	Timer timer;

	if (!loadModel(path))
	{
		std::cerr << "ERROR::MODEL::Failed to load model" << std::endl;
	}

	std::cout << "Model " << name << " loaded in " << timer.Elapsed() << "s" << std::endl;

	// 创建顶点数组对象
	va = std::make_unique<VertexArray>();
	vb = std::make_unique<VertexBuffer>(&vertices[0], vertices.size() * sizeof(Vertex));
	layout = std::make_unique<VertexBufferLayout>();
	layout->Push<float>(3);
	layout->Push<float>(3);
	va->AddBuffer(*vb, *layout);

	// 创建索引缓冲对象
	ib = std::make_unique<IndexBuffer>(&indices[0], indices.size());
}

Model::~Model()
{
}

void Model::Bind() const
{
	va->Bind();
	ib->Bind();
}

void Model::Unbind() const
{
	va->Unbind();
	ib->Unbind();
}

bool Model::loadModel(const std::string& path)
{
	// 读取模型文件
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	// 检查是否读取成功
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return false;
	}

	// 从文件路径中提取目录和文件名
	directory = path.substr(0, path.find_last_of('/'));
	name = path.substr(path.find_last_of('/') + 1);

	ProcessNode(scene->mRootNode, scene);
	
	return true;
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	// 处理节点中的所有网格
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// 获取网格
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		// 法线数据
		if (mesh->HasNormals())
		{
			for (unsigned int j = 0; j < mesh->mNumVertices; j++)
			{
				Vertex vertex;
				vertex.position = glm::vec3(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z);
				vertex.normal = glm::vec3(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z);
				vertices.push_back(vertex);
			}
		}
		else
		{
			std::cerr << "ERROR::MODEL::Mesh does not contain normal data" << std::endl;
		}

		// 处理索引
		for (unsigned int j = 0; j < mesh->mNumFaces; j++)
		{
			aiFace face = mesh->mFaces[j];
			for (unsigned int k = 0; k < face.mNumIndices; k++)
			{
				indices.push_back(face.mIndices[k]);
			}
		}
	}

	// 递归处理子节点
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}
