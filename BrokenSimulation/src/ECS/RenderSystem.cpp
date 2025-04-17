#include "bspch.h"
#include "ECS/RenderSystem.h"

#include "ECS/LightComponent.h"
#include "ECS/ModelComponent.h"
#include "Utils/Utils.h"

namespace BrokenSim
{
	RenderSystem::RenderSystem(std::shared_ptr<ResourceManager> resMgr)
	{
		m_ResourceManager = resMgr;
		m_CurrentShader = resMgr->GetDefaultShader();
		m_VoronoiShader = resMgr->GetVoronoiShader();
		m_VoronoiMappingShader = resMgr->GetVoronoiMappingShader();

		// ���û��
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// ������Ȳ��Ժͱ����޳�
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		// ��ʼ��Voronoiͼ�Ķ�������
		InitVoronoiVertexArray();
	}

	void RenderSystem::InitVoronoiVertexArray()
	{
		float positions[] = {
			-1.0f, -1.0f,
			 1.0f, -1.0f,
			 1.0f,  1.0f,
			-1.0f,  1.0f
		};

		unsigned int indicie[] = {
			0, 1, 2,
			2, 3, 0
		};

		m_VoronoiVertexArray = std::make_shared<VertexArray>();
		std::shared_ptr<VertexBuffer> vb = std::make_shared<VertexBuffer>(positions, sizeof(positions));
		std::shared_ptr<IndexBuffer> ib = std::make_shared<IndexBuffer>(indicie, sizeof(indicie) / sizeof(unsigned int));
		VertexBufferLayout layout({
			{ DataType::Float2, "a_Position" }
			});
		vb->SetLayout(layout);
		m_VoronoiVertexArray->AddVertexBuffer(vb);
		m_VoronoiVertexArray->SetIndexBuffer(ib);
	}

	void RenderSystem::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void RenderSystem::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RenderSystem::SetCurrentShader(const std::string& name)
	{
		m_CurrentShader = m_ResourceManager->GetShaderResource(name);
	}

	std::shared_ptr<Shader> RenderSystem::GetCurrentShader() const
	{
		return m_CurrentShader;
	}

	void RenderSystem::OnUpdate(TimeStep ts, Scene* scene)
	{
		SceneCamera* camera = scene->GetCamera();

		m_CurrentShader->Bind();
		m_CurrentShader->SetUniformMat4f("u_ViewProjection", camera->GetViewProjectionMatrix());
		m_CurrentShader->SetUniform3f("u_ViewPos", camera->GetPosition());

		// ������ѡ�е�ʵ�壬����VoronoiMappingShader����ͼͶӰ�������ͼλ��
		if (m_CurrentEntity)
		{
			m_VoronoiMappingShader->Bind();

			m_VoronoiMappingShader->SetUniformMat4f("u_ViewProjection", camera->GetViewProjectionMatrix());
			m_VoronoiMappingShader->SetUniform3f("u_ViewPos", camera->GetPosition());

			m_CurrentShader->Bind();
		}

		std::vector<LightComponent*> lights = scene->GetLights();
		// ���������û�й�Դ��ʹ��Ĭ�Ϲ�Դ
		if (lights.empty())
		{
			float lightOffsetX = 0.0f;
			float lightOffsetY = 1.0f;

			glm::vec3 lightPos = camera->GetPosition()
				+ camera->GetRightDirection() * lightOffsetX
				+ camera->GetUpDirection() * lightOffsetY;

			m_CurrentShader->SetUniform1i("u_LightCount", 1);
			m_CurrentShader->SetUniform1i("u_Lights[0].Type", (int)LightComponent::LightType::Point);
			m_CurrentShader->SetUniform3f("u_Lights[0].Position", lightPos);
			m_CurrentShader->SetUniform3f("u_Lights[0].Color", glm::vec3(1.0f, 1.0f, 1.0f));

			// ������ѡ�е�ʵ�壬����VoronoiMappingShader�Ĺ�Դ
			if (m_CurrentEntity)
			{
				m_VoronoiMappingShader->Bind();

				m_VoronoiMappingShader->SetUniform1i("u_LightCount", 1);
				m_VoronoiMappingShader->SetUniform1i("u_Lights[0].Type", (int)LightComponent::LightType::Point);
				m_VoronoiMappingShader->SetUniform3f("u_Lights[0].Position", lightPos);
				m_VoronoiMappingShader->SetUniform3f("u_Lights[0].Color", glm::vec3(1.0f, 1.0f, 1.0f));

				m_CurrentShader->Bind();
			}
		}
		else
		{
			m_CurrentShader->SetUniform1i("u_LightCount", lights.size());
			// ���ù�Դ
			for (int i = 0; i < lights.size(); i++)
			{
				std::string lightName = "u_Lights[" + std::to_string(i) + "]";
				m_CurrentShader->SetUniform1i(lightName + ".Type", (int)lights[i]->GetType());
				m_CurrentShader->SetUniform3f(lightName + ".Position", lights[i]->GetPosition());
				m_CurrentShader->SetUniform3f(lightName + ".Color", lights[i]->GetColor());
			}

			// ������ѡ�е�ʵ�壬����VoronoiMappingShader�Ĺ�Դ
			if (m_CurrentEntity)
			{
				m_VoronoiMappingShader->Bind();

				m_VoronoiMappingShader->SetUniform1i("u_LightCount", lights.size());
				// ���ù�Դ
				for (int i = 0; i < lights.size(); i++)
				{
					std::string lightName = "u_Lights[" + std::to_string(i) + "]";
					m_VoronoiMappingShader->SetUniform1i(lightName + ".Type", (int)lights[i]->GetType());
					m_VoronoiMappingShader->SetUniform3f(lightName + ".Position", lights[i]->GetPosition());
					m_VoronoiMappingShader->SetUniform3f(lightName + ".Color", lights[i]->GetColor());
				}

				m_CurrentShader->Bind();
			}
		}
		// ��Ⱦ�����е�����ʵ��
		std::vector<Entity*> entities = scene->GetEntities();

		glm::mat4 rootMatrix = scene->GetRootEntity()->GetModelMatrix();

		for (Entity* entity : entities)
		{
			RenderEntity(ts, entity, rootMatrix);
		}
	}

	void RenderSystem::OnWindowResize(unsigned int width, unsigned int height)
	{
		m_Width = width;
		m_Height = height;
		glViewport(0, 0, width, height);
	}

	void RenderSystem::RenderVoronoi(TimeStep ts, VoronoiComponent* voronoiComponent)
	{
		m_VoronoiShader->Bind();
		// ��ȾVoronoiͼ
		unsigned int numPoints = voronoiComponent->GetNumPoints();

		m_VoronoiShader->SetUniform1i("u_NumPoints", numPoints);

		for (unsigned int i = 0; i < numPoints; i++)
		{
			m_VoronoiShader->SetUniform3f("u_Colors[" + std::to_string(i) + "]", voronoiComponent->GetColor(i));
			m_VoronoiShader->SetUniform2f("u_Points[" + std::to_string(i) + "]", voronoiComponent->GetPoint(i));
		}

		m_VoronoiShader->SetUniform2f("u_ViewportSize", voronoiComponent->GetViewportSize());

		DrawVertexArray(m_VoronoiVertexArray.get());

		m_CurrentShader->Bind();
	}

	void RenderSystem::SetCurrentEntity(Entity* entity)
	{
		m_CurrentEntity = entity;
	}

	Entity* RenderSystem::GetCurrentEntity() const
	{
		return m_CurrentEntity;
	}

	void RenderSystem::RenderEntity(TimeStep ts, Entity* entity, glm::mat4 parentMatrix)
	{
		if (!entity->IsVisible())
		{
			return;
		}


		// ��ȡʵ���ģ�;���
		glm::mat4 entityModelMatrix = parentMatrix * entity->GetModelMatrix();

		if (entity != m_CurrentEntity)
		{
			// ��Ⱦģ��
			if (entity->HasComponent<ModelComponent>())
			{
				ModelComponent* model = entity->GetComponent<ModelComponent>();

				glm::mat4 transform = model->GetTransform();

				glm::mat4 modelMatrix = entityModelMatrix * transform;
				glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelMatrix)));

				// ����uniform
				m_CurrentShader->SetUniformMat4f("u_Model", modelMatrix);
				m_CurrentShader->SetUniformMat3f("u_NormalMatrix", normalMatrix);

				// ���ò���
				m_CurrentShader->SetUniform4f("u_Color", model->GetColor());
				m_CurrentShader->SetUniform1f("u_Shininess", model->GetShininess());
				m_CurrentShader->SetUniform1f("u_AmbientStrength", model->GetAmbientStrength());
				m_CurrentShader->SetUniform1f("u_DiffuseStrength", model->GetDiffuseStrength());
				m_CurrentShader->SetUniform1f("u_SpecularStrength", model->GetSpecularStrength());

				VertexArray* va = model->GetVertexArray();
				DrawVertexArray(va);
			}
		}
		else
		{
			m_VoronoiMappingShader->Bind();

			if (entity->HasComponent<VoronoiComponent>())
			{
				VoronoiComponent* voronoiComponent = entity->GetComponent<VoronoiComponent>();
				unsigned int numPoints = voronoiComponent->GetNumPoints();

				m_VoronoiMappingShader->SetUniform1i("u_NumPoints", numPoints);

				for (unsigned int i = 0; i < numPoints; i++)
				{
					m_VoronoiMappingShader->SetUniform3f("u_Colors[" + std::to_string(i) + "]", voronoiComponent->GetColor(i));
					m_VoronoiMappingShader->SetUniform3f("u_Points[" + std::to_string(i) + "]", voronoiComponent->GetPoint(i));
				}
			}

			if (entity->HasComponent<ModelComponent>())
			{
				ModelComponent* model = entity->GetComponent<ModelComponent>();

				glm::mat4 transform = model->GetTransform();

				glm::mat4 modelMatrix = entityModelMatrix * transform;
				glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelMatrix)));

				// ����uniform
				m_VoronoiMappingShader->SetUniformMat4f("u_Model", modelMatrix);
				m_VoronoiMappingShader->SetUniformMat3f("u_NormalMatrix", normalMatrix);

				// ����ģ�ͼ�������
				m_VoronoiMappingShader->SetUniform3f("u_GeometryCenter", model->GetGeometryCenter());

				// ����ģ��y����Сֵ
				m_VoronoiMappingShader->SetUniform1f("u_YMin", model->GetYMin());
				// ����ģ��y�����ֵ
				m_VoronoiMappingShader->SetUniform1f("u_YMax", model->GetYMax());

				// ���ò���
				m_VoronoiMappingShader->SetUniform4f("u_Color", model->GetColor());
				m_VoronoiMappingShader->SetUniform1f("u_Shininess", model->GetShininess());
				m_VoronoiMappingShader->SetUniform1f("u_AmbientStrength", model->GetAmbientStrength());
				m_VoronoiMappingShader->SetUniform1f("u_DiffuseStrength", model->GetDiffuseStrength());
				m_VoronoiMappingShader->SetUniform1f("u_SpecularStrength", model->GetSpecularStrength());

				VertexArray* va = model->GetVertexArray();
				DrawVertexArray(va);
			}

			m_CurrentShader->Bind();
		}

		// �ݹ���Ⱦ�Ӷ���
		for (Entity* child : entity->GetChildren())
		{
			RenderEntity(ts, child, entityModelMatrix);
		}
	}

	void RenderSystem::DrawVertexArray(VertexArray* va)
	{
		va->Bind();
		va->GetIndexBuffer()->Bind();
		unsigned int indexCount = va->GetIndexBuffer()->GetCount();

		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
	}
}
