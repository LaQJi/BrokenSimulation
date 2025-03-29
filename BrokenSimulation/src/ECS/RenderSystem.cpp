#include "bspch.h"
#include "ECS/RenderSystem.h"

#include "ECS/ModelComponent.h"
#include "Utils/Utils.h"

namespace BrokenSim
{
	RenderSystem::RenderSystem(std::shared_ptr<ResourceManager> resMgr)
	{
		m_ResourceManager = resMgr;
		m_CurrentShader = resMgr->GetDefaultShader();
		m_VoronoiShader = resMgr->GetVoronoiShader();
		
		// ���û��
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		// ������Ȳ��Ժͱ����޳�
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glEnable(GL_CULL_FACE));

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
		GLCall(glClearColor(color.r, color.g, color.b, color.a));		
	}

	void RenderSystem::Clear()
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void RenderSystem::SetShader(const std::string& name)
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

		std::vector<LightComponent*> lights = scene->GetLights();
		// ���������û�й�Դ�����һ��Ĭ�Ϲ�Դ
		if (lights.empty())
		{
			float lightOffsetX = 2.0f;
			float lightOffsetY = 2.0f;

			glm::vec3 lightPos = camera->GetPosition()
				+ camera->GetRightDirection() * lightOffsetX
				+ camera->GetUpDirection() * lightOffsetY;

			LightComponent* light = new LightComponent(nullptr, LightComponent::LightType::Point, lightPos, glm::vec3(1.0f, 1.0f, 1.0f));
			lights.push_back(light);
			
			// ��ӵ�������
			scene->AddLight(light);
		}

		// ���ù�Դ
		for (int i = 0; i < lights.size(); i++)
		{
			std::string lightName = "u_Lights[" + std::to_string(i) + "]";
			m_CurrentShader->SetUniform1i(lightName + ".Type", (int)lights[i]->GetType());
			m_CurrentShader->SetUniform3f(lightName + ".Position", lights[i]->GetPosition());
			m_CurrentShader->SetUniform3f(lightName + ".Color", lights[i]->GetColor());
		}

		// ��Ⱦ�����е�����ʵ��
		std::vector<Entity*> entities = scene->GetEntities();

		for (Entity* entity : entities)
		{
			RenderEntity(ts, entity, glm::mat4(1.0f));
		}
	}

	void RenderSystem::OnUpdate(TimeStep ts, VoronoiComponent* voronoiComponent)
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
	}

	void RenderSystem::RenderEntity(TimeStep ts, Entity* entity, glm::mat4 parentMatrix)
	{
		// ��ȡʵ���ģ�;���
		glm::mat4 modelMatrix = parentMatrix * entity->GetModelMatrix();
		
		glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelMatrix)));

		// ����uniform
		m_CurrentShader->SetUniformMat4f("u_Model", modelMatrix);
		m_CurrentShader->SetUniformMat3f("u_NormalMatrix", normalMatrix);

		// TODO: �����������Ӱ�촫�ݸ���Ⱦ���
		// ��Ⱦģ��
		if (entity->HasComponent<ModelComponent>())
		{
			ModelComponent* model = entity->GetComponent<ModelComponent>();

			// ���ò���
			m_CurrentShader->SetUniform4f("u_Color", model->GetColor());
			m_CurrentShader->SetUniform1f("u_Shininess", model->GetShininess());
			m_CurrentShader->SetUniform1f("u_AmbientStrength", model->GetAmbientStrength());
			m_CurrentShader->SetUniform1f("u_DiffuseStrength", model->GetDiffuseStrength());
			m_CurrentShader->SetUniform1f("u_SpecularStrength", model->GetSpecularStrength());

			VertexArray* va = model->GetVertexArray();
			DrawVertexArray(va);
		}

		// �ݹ���Ⱦ�Ӷ���
		for (Entity* child : entity->GetChildren())
		{
			RenderEntity(ts, child, modelMatrix);
		}
	}

	void RenderSystem::DrawVertexArray(VertexArray* va)
	{
		va->Bind();
		va->GetIndexBuffer()->Bind();
		unsigned int indexCount = va->GetIndexBuffer()->GetCount();

		GLCall(glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr));
	}
}
