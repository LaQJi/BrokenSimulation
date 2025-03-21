#include "bspch.h"
#include "Scene/ModelObject.h"

namespace BrokenSim
{
	ModelObject::ModelObject(unsigned int id, const std::string& path, Object* parent, const std::string& name)
	{
		this->id = id;
		this->type = Type::Model;
		this->m_Path = path;
		this->SetParent(parent);
		this->SetName(name);

		if (this->LoadModel(path))
		{
			BS_CORE_INFO("Model \' {0} \' loaded successfully!", m_Path);
		}
		else
		{
			BS_CORE_ERROR("Failed to load model from path \' {0} \'!", m_Path);
		}
	}

	ModelObject::~ModelObject()
	{
	}

	void ModelObject::Bind() const
	{
	}

	void ModelObject::Unbind() const
	{
	}

	void ModelObject::OnUpdate(TimeStep ts)
	{
	}

	void ModelObject::OnRender()
	{
	}

	void ModelObject::OnEvent(Event& e)
	{
	}

	bool ModelObject::LoadModel(const std::string& path)
	{
		// 读取模型
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
		// 检查是否加载成功
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			BS_CORE_ERROR("ERROR::ASSIMP::{0}", importer.GetErrorString());
			return false;
		}

		// 处理场景中的所有节点
		this->ProcessNode(scene->mRootNode, scene);

		return true;
	}

	void ModelObject::ProcessNode(aiNode* node, const aiScene* scene)
	{
		// 处理节点所有的网格
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			// 获取网格
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			// TODO: 处理网格,由于va实现与过往不同，需要根据现有va处理网格数据
		}
	}
}
