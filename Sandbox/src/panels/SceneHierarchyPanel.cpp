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

		// 渲染场景中的所有实体
		if (m_Context)
		{
			const Entity* root = m_Context->GetRootEntity();
			const char* name = root->GetName().empty()
				? "Root" : root->GetName().c_str();

			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow
									| ImGuiTreeNodeFlags_SpanAvailWidth;

			if (ImGui::IsItemClicked)
			{
				// 高亮选中的实体
				flags |= ImGuiTreeNodeFlags_Selected;
			}

			if (root->GetChildren().empty())
			{
				flags |= ImGuiTreeNodeFlags_Leaf;
			}
			
			// 创建TreeNode
			bool opened = ImGui::TreeNodeEx((void*)(intptr_t)root->GetID(), flags, root->GetName().c_str());

			// 处理选中
			if (ImGui::IsItemClicked())
			{
				m_SelectionContext = const_cast<Entity*>(root);
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
				// 递归渲染子实体
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

		ImGui::Begin("Properties");

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
			// 高亮选中的实体
			flags |= ImGuiTreeNodeFlags_Selected;
		}

		if (entity->GetChildren().empty())
		{
			flags |= ImGuiTreeNodeFlags_Leaf;
		}

		// 创建TreeNode
		bool opened = ImGui::TreeNodeEx((void*)(intptr_t)entity->GetID(), flags, name);
		
		// 处理选中
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
			// 递归渲染子实体
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
		if (entity->HasComponent<ModelComponent>())
		{
			auto* mc = entity->GetComponent<ModelComponent>();
			
			ImGui::Text("Model Component");

			ImGui::Separator();

			ImGui::Text("Transform Properties");
			ImGui::DragFloat3("Position", &mc->GetPosition().x, 0.1f);
			ImGui::DragFloat3("Rotation", &mc->GetRotation().x, 0.1f);
			ImGui::DragFloat3("Scale", &mc->GetScale().x, 0.1f);

			ImGui::Separator();

			ImGui::Text("Texture Properties");
			ImGui::ColorEdit4("Color", glm::value_ptr(mc->GetColor()));
			ImGui::DragFloat("Shininess", &mc->GetShininess(), 0.0f, 256.0f);
			ImGui::DragFloat("Ambient Strength", &mc->GetAmbientStrength(), 0.0f, 1.0f);
			ImGui::DragFloat("Diffuse Strength", &mc->GetDiffuseStrength(), 0.0f, 1.0f);
			ImGui::DragFloat("Specular Strength", &mc->GetSpecularStrength(), 0.0f, 1.0f);

			ImGui::Separator();

			if (ImGui::Button("Delete Component"))
			{
				entity->RemoveComponent<ModelComponent>();
			}
		}
	}
}
