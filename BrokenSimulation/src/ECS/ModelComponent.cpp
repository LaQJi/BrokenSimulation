#include "bspch.h"
#include "ECS/ModelComponent.h"

#include "Core/Log.h"
#include "Core/Macros.h"

#include "ECS/Entity.h"

namespace BrokenSim
{
	ModelComponent::ModelComponent(Entity* owner, const std::string& path)
		: Component(owner), m_Path(path)
	{
		m_BoudingBox.minPoint = glm::vec3(FLT_MAX);
		m_BoudingBox.maxPoint = glm::vec3(FLT_MIN);
		if (LoadModel(m_Path))
		{
			if (m_Vertices.size() > 0)
			{
				// 计算几何中心
				m_GeometryCenter /= m_Vertices.size();

				// 创建顶点数组
				m_VertexArray = std::make_unique<VertexArray>();
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
			BS_CORE_ASSERT(false, "Failed to load model from path!");
		}
	}

	ModelComponent::ModelComponent(Entity* owner, const Meshes& meshes)
		: Component(owner), m_Path("")
	{
		m_Vertices = meshes.vertices;
		m_Indices = meshes.indices;

		if (m_Vertices.size() > 0)
		{
			// 计算几何中心
			m_GeometryCenter /= m_Vertices.size();

			// 创建顶点数组
			m_VertexArray = std::make_unique<VertexArray>();
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
	}

	ModelComponent::~ModelComponent()
	{
	}

	void ModelComponent::OnUpdate(TimeStep ts)
	{
	}

	void ModelComponent::UpdateMeshes(const Meshes& meshes)
	{
		m_Vertices = meshes.vertices;
		m_Indices = meshes.indices;

		if (m_Vertices.size() > 0)
		{
			// 计算几何中心
			m_GeometryCenter /= m_Vertices.size();
			// 创建顶点数组
			m_VertexArray = std::make_unique<VertexArray>();
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
	}

	glm::vec4& ModelComponent::GetColor()
	{
		return m_Color;
	}

	void ModelComponent::SetColor(const glm::vec4& color)
	{
		m_Color = color;
	}

	float& ModelComponent::GetShininess()
	{
		return m_Shininess;
	}

	void ModelComponent::SetShininess(float shininess)
	{
		m_Shininess = shininess;
	}

	float& ModelComponent::GetAmbientStrength()
	{
		return m_AmbientStrength;
	}

	void ModelComponent::SetAmbientStrength(float strength)
	{
		m_AmbientStrength = strength;
	}

	float& ModelComponent::GetDiffuseStrength()
	{
		return m_DiffuseStrength;
	}

	void ModelComponent::SetDiffuseStrength(float strength)
	{
		m_DiffuseStrength = strength;
	}

	float& ModelComponent::GetSpecularStrength()
	{
		return m_SpecularStrength;
	}

	void ModelComponent::SetSpecularStrength(float strength)
	{
		m_SpecularStrength = strength;
	}

	const glm::vec3& ModelComponent::GetGeometryCenter() const
	{
		return m_GeometryCenter;
	}

	const std::string& ModelComponent::GetPath() const
	{
		return m_Path;
	}

	float ModelComponent::GetYMin() const
	{
		return yMin;
	}

	float ModelComponent::GetYMax() const
	{
		return yMax;
	}

	const std::vector<Vertex>& ModelComponent::GetVertices() const
	{
		return m_Vertices;
	}

	const std::vector<unsigned int>& ModelComponent::GetIndices() const
	{
		return m_Indices;
	}

	const AABB& ModelComponent::GetBoundingBox() const
	{
		return m_BoudingBox;
	}

	bool ModelComponent::LoadModel(const std::string& path)
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

		// 处理节点
		ProcessNode(scene->mRootNode, scene);
		
		return true;
	}

	void ModelComponent::ProcessNode(aiNode* node, const aiScene* scene)
	{
		// 处理节点所有的网格
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			// 获取网格
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			if (mesh->mNumVertices == 0)
			{
				BS_CORE_WARN("Mesh has no vertices!");
				continue;
			}
			// 处理网格中的所有顶点
			for (unsigned int j = 0; j < mesh->mNumVertices; j++)
			{
				Vertex vertex;
				// 顶点坐标
				vertex.Position.x = mesh->mVertices[j].x;
				vertex.Position.y = mesh->mVertices[j].y;
				vertex.Position.z = mesh->mVertices[j].z;

				// 计算几何中心
				m_GeometryCenter += glm::vec3(vertex.Position.x, vertex.Position.y, vertex.Position.z);

				// 计算y轴范围
				if (vertex.Position.y < yMin)
				{
					yMin = vertex.Position.y;
				}
				if (vertex.Position.y > yMax)
				{
					yMax = vertex.Position.y;
				}

				if (vertex.Position.x < m_BoudingBox.minPoint.x)
				{
					m_BoudingBox.minPoint.x = vertex.Position.x;
				}
				if (vertex.Position.x > m_BoudingBox.maxPoint.x)
				{
					m_BoudingBox.maxPoint.x = vertex.Position.x;
				}

				if (vertex.Position.y < m_BoudingBox.minPoint.y)
				{
					m_BoudingBox.minPoint.y = vertex.Position.y;
				}
				if (vertex.Position.y > m_BoudingBox.maxPoint.y)
				{
					m_BoudingBox.maxPoint.y = vertex.Position.y;
				}

				if (vertex.Position.z < m_BoudingBox.minPoint.z)
				{
					m_BoudingBox.minPoint.z = vertex.Position.z;
				}
				if (vertex.Position.z > m_BoudingBox.maxPoint.z)
				{
					m_BoudingBox.maxPoint.z = vertex.Position.z;
				}

				// 顶点法线
				vertex.Normal.x = mesh->mNormals[j].x;
				vertex.Normal.y = mesh->mNormals[j].y;
				vertex.Normal.z = mesh->mNormals[j].z;
				// 顶点纹理坐标
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
			// 处理网格中的所有面
			for (unsigned int j = 0; j < mesh->mNumFaces; j++)
			{
				aiFace face = mesh->mFaces[j];
				// 处理面中的所有索引
				for (unsigned int k = 0; k < face.mNumIndices; k++)
				{
					m_Indices.push_back(face.mIndices[k]);
				}
			}
		}

		// 处理节点的所有子节点
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			this->ProcessNode(node->mChildren[i], scene);
		}
	}
}
