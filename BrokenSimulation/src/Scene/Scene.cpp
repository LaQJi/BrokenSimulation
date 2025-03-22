#include "bspch.h"
#include "Scene/Scene.h"

#include "Core/Log.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace BrokenSim
{
	Scene::Scene()
	{
		// 初始化ID
		m_AvailableIDs.push(0);

		m_Shader = std::make_shared<Shader>("res/Shaders/scene_v.shader", "res/Shaders/scene_f.shader");
	}

	Scene::~Scene()
	{
	}

	void Scene::OnUpdate(TimeStep ts, Camera& camera)
	{
		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_ViewProjection", camera.GetViewProjectionMatrix());
		m_Shader->SetUniform3f("u_ViewPos", camera.GetPosition());
		

		// 更新所有Object
		for (auto& object : m_Objects)
		{
			if (object.second->GetEnabled())
			{
				std::shared_ptr<ModelObject> modelObject = std::dynamic_pointer_cast<ModelObject>(object.second);
				if (modelObject)
				{
					modelObject->OnUpdate(ts, m_Shader);
				}
			}
		}
	}


	std::shared_ptr<EmptyObject> Scene::CreateEmptyObject(const Object::Type& type, const std::string& name, Object* parent)
	{
		// 获取ID
		unsigned int id = m_AvailableIDs.top();
		m_AvailableIDs.pop();

		if (m_AvailableIDs.empty())
		{
			m_AvailableIDs.push(id + 1);
		}

		if (parent != nullptr)
		{
			// 查找父Object是否在Scene中
			Object* root = parent;
			while (root->GetParent() != nullptr)
			{
				root = root->GetParent();
			}
			unsigned int rootID = root->GetID();

			if (m_Objects.find(rootID) == m_Objects.end())
			{
				BS_CORE_ERROR("Parent Object not found in Scene!");
				return std::shared_ptr<EmptyObject>();
			}
			else if (m_Objects[rootID].get() != root)
			{
				BS_CORE_ERROR("Parent Object not found in Scene!");
				return std::shared_ptr<EmptyObject>();
			}
			else
			{
				// 创建Object
				std::shared_ptr<EmptyObject> object = std::make_shared<EmptyObject>(id, name, parent);
				m_Objects[id] = object;
				parent->AddChild(object);
				return object;
			}
		}
		else
		{
			// 创建Object
			std::shared_ptr<EmptyObject> object = std::make_shared<EmptyObject>(id, name, parent);
			m_Objects[id] = object;
			return object;
		}
	}

	std::shared_ptr<ModelObject> Scene::CreateModelObject(const Object::Type& type, const std::string& path, const std::string& name, Object* parent)
	{
		// 获取ID
		unsigned int id = m_AvailableIDs.top();
		m_AvailableIDs.pop();

		if (m_AvailableIDs.empty())
		{
			m_AvailableIDs.push(id + 1);
		}

		if (parent != nullptr)
		{
			// 查找父Object是否在Scene中
			Object* root = parent;
			while (root->GetParent() != nullptr)
			{
				root = root->GetParent();
			}
			unsigned int rootID = root->GetID();
			if (m_Objects.find(rootID) == m_Objects.end())
			{
				BS_CORE_ERROR("Parent Object not found in Scene!");
				return std::shared_ptr<ModelObject>();
			}
			else if (m_Objects[rootID].get() != root)
			{
				BS_CORE_ERROR("Parent Object not found in Scene!");
				return std::shared_ptr<ModelObject>();
			}
			else
			{
				// 创建Object
				std::shared_ptr<ModelObject> object = std::make_shared<ModelObject>(id, name, parent, path);
				m_Objects[id] = object;
				parent->AddChild(object);
				return object;
			}
		}
		else
		{
			// 创建Object
			std::shared_ptr<ModelObject> object = std::make_shared<ModelObject>(id, name, parent, path);
			m_Objects[id] = object;
			return object;
		}
	}

	void Scene::DeleteObject(std::shared_ptr<Object> object)
	{
		std::vector<unsigned int> ids;
		// 查找根Object是否在Scene中
		Object* root = object.get();
		while (root->GetParent() != nullptr)
		{
			root = root->GetParent();
		}
		unsigned int rootID = root->GetID();

		if (m_Objects.find(rootID) == m_Objects.end())
		{
			BS_CORE_ERROR("Object not found in Scene!");
			return;
		}
		else if (m_Objects[rootID].get() != root)
		{
			BS_CORE_ERROR("Object not found in Scene!");
			return;
		}
		else
		{
			// 删除Object及其所有子Object
			ids.push_back(rootID);

			if (object->GetChildren().size() > 0)
			{
				std::vector<unsigned int> childrenIDs = object->RemoveAllChildren();
			}

			ids.insert(ids.end(), object->RemoveAllChildren().begin(), object->RemoveAllChildren().end());

			for (auto& id : ids)
			{
				m_AvailableIDs.push(id);
			}

			// 若Object为根Object，则从Scene中删除
			if (object->GetParent() == nullptr)
			{
				m_Objects.erase(object->GetID());
			}
		}
	}

	void Scene::OnImGuiRender()
	{
		for (auto object : m_Objects)
		{
			ImGui::Text("Object Name: %d", object.second->GetName());
			ImGui::SliderFloat3("Position", glm::value_ptr(object.second->GetPosition()), -10.0f, 10.0f);
			ImGui::SliderFloat3("Rotation", glm::value_ptr(object.second->GetRotation()), -180.0f, 180.0f);
			ImGui::SliderFloat3("Scale", glm::value_ptr(object.second->GetScale()), 0.0f, 10.0f);
		}
	}
}
