#include "bspch.h"
#include "Scene/ModelObject.h"

#include "Renderer/Renderer.h"

namespace BrokenSim
{
	ModelObject::ModelObject(unsigned int id, const std::string& name, Object* parent, const std::string path)
	{
		this->id = id;
		this->type = Type::Model;
		this->m_Path = path;
		this->SetParent(parent);
		this->SetName(name);

		if (this->LoadModel(path))
		{
			if (m_Vertices.size() > 0)
			{
				// ������������
				m_VertexArray = std::make_shared<VertexArray>();
				std::shared_ptr<VertexBuffer> vb = std::make_shared<VertexBuffer>(&m_Vertices[0].Position.x, m_Vertices.size() * sizeof(Vertex));
				std::shared_ptr<IndexBuffer> ib = std::make_shared<IndexBuffer>(&m_Indices[0], m_Indices.size());
				VertexBufferLayout layout({
					{ DataType::Float3, "a_Position" },
					{ DataType::Float3, "a_Normal" },
					{ DataType::Float2, "a_TexCoords" }
					});
				vb->SetLayout(layout);
				m_VertexArray->AddVertexBuffer(vb);
				m_VertexArray->SetIndexBuffer(ib);
			}
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

	void ModelObject::OnUpdate(TimeStep ts, std::shared_ptr<Shader>& shader)
	{
	}

	void ModelObject::OnRender(std::shared_ptr<Shader> shader)
	{
		// ����ģ�;���
		glm::mat4 model = this->GetModelMatrix();

		// ��ȡ�������ģ�;���
		Object* parent = this->GetParent();
		while (parent != nullptr)
		{
			// ����ģ�;���
			model = parent->GetModelMatrix() * model;

			parent = parent->GetParent();
		}
		glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));

		// ����uniform
		shader->SetUniformMat4f("u_Transform", model);
		shader->SetUniformMat3f("u_NormalMatrix", normalMatrix);
		shader->SetUniform4f("u_Color", m_Color);
		shader->SetUniform1f("u_Shininess", m_Shininess);
		shader->SetUniform1f("u_AmbientStrength", m_AmbientStrength);
		shader->SetUniform1f("u_DiffuseStrength", m_DiffuseStrength);
		shader->SetUniform1f("u_SpecularStrength", m_SpecularStrength);

		Renderer::DrawIndexed(m_VertexArray);

	}

	void ModelObject::OnEvent(Event& e)
	{
	}

	bool ModelObject::LoadModel(const std::string& path)
	{
		// ��ȡģ��
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
		// ����Ƿ���سɹ�
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			BS_CORE_ERROR("ERROR::ASSIMP::{0}", importer.GetErrorString());
			return false;
		}

		// �������е����нڵ�
		this->ProcessNode(scene->mRootNode, scene);

		return true;
	}

	void ModelObject::ProcessNode(aiNode* node, const aiScene* scene)
	{
		// ����ڵ����е�����
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			// ��ȡ����
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			if (mesh->mNumVertices == 0)
			{
				BS_CORE_WARN("Mesh has no vertices!");
				continue;
			}
			// ���������е����ж���
			for (unsigned int j = 0; j < mesh->mNumVertices; j++)
			{
				Vertex vertex;
				// ��������
				vertex.Position.x = mesh->mVertices[j].x;
				vertex.Position.y = mesh->mVertices[j].y;
				vertex.Position.z = mesh->mVertices[j].z;

				// ���㷨��
				vertex.Normal.x = mesh->mNormals[j].x;
				vertex.Normal.y = mesh->mNormals[j].y;
				vertex.Normal.z = mesh->mNormals[j].z;

				// ������������
				if (mesh->mTextureCoords[0])
				{
					vertex.TexCoords.x = mesh->mTextureCoords[0][j].x;
					vertex.TexCoords.y = mesh->mTextureCoords[0][j].y;
				}
				else
				{
					vertex.TexCoords = glm::vec2(0.0f, 0.0f);
				}

				m_Vertices.push_back(vertex);
			}

			// ���������е�������
			for (unsigned int j = 0; j < mesh->mNumFaces; j++)
			{
				aiFace face = mesh->mFaces[j];
				// �������е���������
				for (unsigned int k = 0; k < face.mNumIndices; k++)
				{
					m_Indices.push_back(face.mIndices[k]);
				}
			}
		}

		// ����ڵ�������ӽڵ�
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			this->ProcessNode(node->mChildren[i], scene);
		}
	}
}
