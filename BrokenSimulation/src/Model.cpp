#include "Model.h"

Model::Model(const std::string& path)
{
	Timer timer;

	if (!loadModel(path))
	{
		std::cerr << "ERROR::MODEL::Failed to load model" << std::endl;
	}

	std::cout << "Model " << name << " loaded in " << timer.Elapsed() << "s" << std::endl;

	// ���������������
	va = std::make_unique<VertexArray>();
	vb = std::make_unique<VertexBuffer>(&vertices[0], vertices.size() * sizeof(Vertex));
	layout = std::make_unique<VertexBufferLayout>();
	layout->Push<float>(3);
	layout->Push<float>(3);
	va->AddBuffer(*vb, *layout);

	// ���������������
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
	// ��ȡģ���ļ�
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	// ����Ƿ��ȡ�ɹ�
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return false;
	}

	// ���ļ�·������ȡĿ¼���ļ���
	directory = path.substr(0, path.find_last_of('/'));
	name = path.substr(path.find_last_of('/') + 1);

	ProcessNode(scene->mRootNode, scene);
	
	return true;
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	// ����ڵ��е���������
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// ��ȡ����
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		// ��������
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

		// ��������
		for (unsigned int j = 0; j < mesh->mNumFaces; j++)
		{
			aiFace face = mesh->mFaces[j];
			for (unsigned int k = 0; k < face.mNumIndices; k++)
			{
				indices.push_back(face.mIndices[k]);
			}
		}
	}

	// �ݹ鴦���ӽڵ�
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}
