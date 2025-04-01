#include "bspch.h"
#include "SceneHierarchyPanel.h"
#include <glm/gtc/type_ptr.hpp>

namespace BrokenSim
{
	SceneHierarchyPanel::SceneHierarchyPanel(const std::shared_ptr<Scene>& context)
	{
		SetContext(context);
	}

	std::shared_ptr<Scene> SceneHierarchyPanel::GetContext() const
	{
		return m_Context;
	}

	void SceneHierarchyPanel::SetContext(const std::shared_ptr<Scene>& context)
	{
		m_Context = context;
		m_SelectionContext = nullptr;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		// ��Ⱦ�����е�����ʵ��
		if (m_Context)
		{
			const Entity* root = m_Context->GetRootEntity();
			const char* name = root->GetName().empty()
				? "Root" : root->GetName().c_str();

			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow
									| ImGuiTreeNodeFlags_SpanAvailWidth;

			if (ImGui::IsItemClicked)
			{
				// ����ѡ�е�ʵ��
				flags |= ImGuiTreeNodeFlags_Selected;
			}

			if (root->GetChildren().empty())
			{
				flags |= ImGuiTreeNodeFlags_Leaf;
			}
			
			// ����TreeNode
			bool opened = ImGui::TreeNodeEx((void*)(intptr_t)root->GetID(), flags, root->GetName().c_str());

			// ����ѡ��
			if (ImGui::IsItemClicked())
			{
				if (!m_SelectionContext)
				{
					m_SelectionContext = const_cast<Entity*>(root);
				}
			}

			bool entityDeleted = false;
			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::MenuItem("Delete Entity"))
				{
					entityDeleted = true;
				}
				ImGui::EndPopup();
			}

			if (opened)
			{
				// �ݹ���Ⱦ��ʵ��
				for (const Entity* entity : root->GetChildren())
				{
					DrawEntityNode(const_cast<Entity*>(entity));
				}
				ImGui::TreePop();
			}

			if (entityDeleted)
			{
				m_Context->DestroyEntity(const_cast<Entity*>(root));
				if (m_SelectionContext == root)
				{
					m_SelectionContext = nullptr;
				}
			}
		}

		ImGui::End();

		ImGui::Begin("Entity Properties");

		if (m_SelectionContext)
		{
			ImGui::Text(m_SelectionContext->GetName().c_str());

			ImGui::Separator();

			ImGui::Text("Transform Properties");

			ImGui::DragFloat3("Position", &m_SelectionContext->GetPosition().x, 0.01f);
			ImGui::DragFloat3("Rotation", &m_SelectionContext->GetRotation().x, 0.5f);
			ImGui::DragFloat3("Scale", &m_SelectionContext->GetScale().x, 0.01f);

			if (ImGui::Button("Reset Transform"))
			{
				m_SelectionContext->SetPosition({ 0.0f, 0.0f, 0.0f });
				m_SelectionContext->SetRotation({ 0.0f, 0.0f, 0.0f });
				m_SelectionContext->SetScale({ 1.0f, 1.0f, 1.0f });
			}

			ImGui::Separator();

			if (ImGui::Button("Add Component"))
			{
				ImGui::OpenPopup("AddComponent");
			}

			if (ImGui::BeginPopup("AddComponent"))
			{
				// ��ʵ���Ѿ���ModelComponent��������VoronoiComponent
				if (m_SelectionContext->HasComponent<ModelComponent>())
				{
					// ��ʵ��û��VoronoiComponent��������VoronoiComponent
					if (!m_SelectionContext->HasComponent<VoronoiComponent>())
					{
						if (ImGui::MenuItem("Voronoi Component"))
						{
							static unsigned int numPoints = 6;
							
							ImGui::DragInt("Num Points",
								(int*)&numPoints,
								1.0f,		// ��ק�ٶ�
								1,			// ��Сֵ
								100,		// ���ֵ
								"%d",		// ��ʽ���ַ���
								ImGuiSliderFlags_AlwaysClamp);	// ǿ�����Ʒ�Χ

							m_SelectionContext->AddComponent<VoronoiComponent>(numPoints);
							ImGui::CloseCurrentPopup();
						}
					}
				}
				else
				{
					// ��ʵ��û��ModelComponent��������ModelComponent
					if (ImGui::MenuItem("Model Component"))
					{
						// ���ļ��Ի���Windows API��
						OPENFILENAMEA ofn;

						char filePath[MAX_PATH] = "";

						ZeroMemory(&ofn, sizeof(ofn));
						ofn.lStructSize = sizeof(ofn);
						ofn.lpstrFile = filePath;
						ofn.nMaxFile = MAX_PATH;
						ofn.lpstrFilter = "Model Files\0*.obj;*.fbx;*.gltf\0";
						ofn.nFilterIndex = 1;
						ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

						// ���ļ��Ի���ֱ��ѡ���ļ���ȡ��
						if (GetOpenFileNameA(&ofn))
						{
							m_SelectionContext->AddComponent<ModelComponent>(filePath);
						}

						ImGui::CloseCurrentPopup();
					}
				}
				ImGui::EndPopup();
			}
		}

		ImGui::End();

		ImGui::Begin("Components Properties");

		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);
		}

		ImGui::End();
	}

	Entity* SceneHierarchyPanel::GetSelectionContext() const
	{
		return m_SelectionContext;
	}

	void SceneHierarchyPanel::SetSelectionContext(const Entity* context)
	{
		m_SelectionContext = const_cast<Entity*>(context);
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity* entity)
	{
		const char* name = entity->GetName().empty()
			? "Entity" : entity->GetName().c_str();

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow
			| ImGuiTreeNodeFlags_SpanAvailWidth;

		if (ImGui::IsItemClicked())
		{
			// ����ѡ�е�ʵ��
			flags |= ImGuiTreeNodeFlags_Selected;
		}

		if (entity->GetChildren().empty())
		{
			flags |= ImGuiTreeNodeFlags_Leaf;
		}

		// ����TreeNode
		bool opened = ImGui::TreeNodeEx((void*)(intptr_t)entity->GetID(), flags, name);
		
		// ����ѡ��
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
			{
				entityDeleted = true;
			}
			ImGui::EndPopup();
		}

		if (opened)
		{
			// �ݹ���Ⱦ��ʵ��
			for (Entity* child : entity->GetChildren())
			{
				DrawEntityNode(child);
			}
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			m_Context->DestroyEntity(entity);
			if (m_SelectionContext == entity)
			{
				m_SelectionContext = nullptr;
			}
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity* entity)
	{
		if  (!entity)
		{
			return;
		}

		ImGui::Separator();

		if (entity->HasComponent<ModelComponent>())
		{
			auto* mc = entity->GetComponent<ModelComponent>();
			
			ImGui::Text("Model Component");

			ImGui::Separator();

			ImGui::Text("Transform Properties");
			ImGui::DragFloat3("Position", &mc->GetPosition().x, 0.01f);
			ImGui::DragFloat3("Rotation", &mc->GetRotation().x, 0.5f);
			ImGui::DragFloat3("Scale", &mc->GetScale().x, 0.01f);

			if (ImGui::Button("Reset Transform"))
			{
				mc->SetPosition({ 0.0f, 0.0f, 0.0f });
				mc->SetRotation({ 0.0f, 0.0f, 0.0f });
				mc->SetScale({ 1.0f, 1.0f, 1.0f });
			}

			ImGui::Separator();

			ImGui::Text("Texture Properties");
			ImGui::ColorEdit4("Color", glm::value_ptr(mc->GetColor()));
			ImGui::DragFloat("Shininess", &mc->GetShininess(), 0.5f, 0.0f, 256.0f);
			ImGui::DragFloat("Ambient Strength", &mc->GetAmbientStrength(), 0.005f, 0.0f, 1.0f);
			ImGui::DragFloat("Diffuse Strength", &mc->GetDiffuseStrength(), 0.005f, 0.0f, 1.0f);
			ImGui::DragFloat("Specular Strength", &mc->GetSpecularStrength(), 0.005f, 0.0f, 1.0f);

			if (ImGui::Button("Reset Texture Properties"))
			{
				mc->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
				mc->SetShininess(32.0f);
				mc->SetAmbientStrength(0.1f);
				mc->SetDiffuseStrength(1.0f);
				mc->SetSpecularStrength(0.5f);
			}

			ImGui::Separator();

			static unsigned int numPoints = 6;

			ImGui::DragInt("Num Points",
				(int*)&numPoints,
				1.0f,		// ��ק�ٶ�
				1,			// ��Сֵ
				100,		// ���ֵ
				"%d",		// ��ʽ���ַ���
				ImGuiSliderFlags_AlwaysClamp);	// ǿ�����Ʒ�Χ


			if (ImGui::Button("Add Voronoi Component"))
			{
				entity->AddComponent<VoronoiComponent>(numPoints);
			}

			ImGui::Separator();

			if (ImGui::Button("Delete Model Component"))
			{
				entity->RemoveComponent<ModelComponent>();
			}
		}
	}
}
