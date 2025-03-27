#include "bspch.h"
#include "Scene/Scene3D.h"

#include "Core/Log.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace BrokenSim
{
	Scene3D::Scene3D()
	{
		std::shared_ptr<Shader> shader = std::make_shared<Shader>("res/Shaders/scene_v.shader", "res/Shaders/scene_f.shader");

		Init(shader);
	}

	Scene3D::~Scene3D()
	{
	}

	void Scene3D::OnUpdate(TimeStep ts, Camera& camera)
	{
		// 渲染场景
		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_ViewProjection", camera.GetViewProjectionMatrix());
		m_Shader->SetUniform3f("u_ViewPos", camera.GetPosition());

		float lightOffsetX = 2.0f;
		float lightOffsetY = 2.0f;

		glm::vec3 lightPos = camera.GetPosition() + camera.GetRight() * lightOffsetX + camera.GetUp() * lightOffsetY;
		m_Shader->SetUniform3f("u_LightPos", lightPos);
		m_Shader->SetUniform3f("u_LightColor", glm::vec3(1.0f, 1.0f, 1.0f));

	}

	void Scene3D::OnRender()
	{
		// 更新所有Object
		for (auto& object : m_Objects)
		{
			if (object.second->GetEnabled())
			{
				std::shared_ptr<ModelObject> modelObject = std::dynamic_pointer_cast<ModelObject>(object.second);
				if (modelObject)
				{
					modelObject->OnRender(m_Shader);
				}
			}
		}
	}

	void Scene3D::OnImGuiRender()
	{
		for (auto object : m_Objects)
		{
			// TODO: 为每个Object添加ImGui面板，完善显示内容
			ImGui::Text("Object Name: %d", object.second->GetName().c_str());

			ImGui::PushID(object.second->GetID());

			ImGui::SliderFloat3("Position", glm::value_ptr(object.second->GetPosition()), -10.0f, 10.0f);
			ImGui::SliderFloat3("Rotation", glm::value_ptr(object.second->GetRotation()), -180.0f, 180.0f);
			ImGui::SliderFloat3("Scale", glm::value_ptr(object.second->GetScale()), 0.0f, 10.0f);

			ImGui::PopID();
		}
	}

	std::shared_ptr<Object> Scene3D::CreateModelObject(const std::string& path, const std::string& name, Object* parent)
	{
		if (parent != nullptr)
		{
			unsigned int parentID = parent->GetID();
			if (auto iter = m_Objects.find(parentID); iter == m_Objects.end())
			{
				BS_CORE_ERROR("Parent object not found!");
				return std::shared_ptr<Object>();
			}
			else if (iter->second->GetType() != Object::Type::Empty)
			{
				BS_CORE_ERROR("Parent object is not an EmptyObject!");
				return std::shared_ptr<Object>();
			}
			else if (parent != iter->second.get())
			{
				BS_CORE_ERROR("Parent object not found!");
				return std::shared_ptr<Object>();
			}
			else
			{
				unsigned int id = AssignID();
				std::shared_ptr<ModelObject> modelObject = std::make_shared<ModelObject>(id, name, parent, path);
				m_Objects[id] = modelObject;
				return modelObject;
			}
		}
		else
		{
			unsigned int id = AssignID();
			std::shared_ptr<ModelObject> modelObject = std::make_shared<ModelObject>(id, name, parent, path);
			m_Objects[id] = modelObject;
			return modelObject;
		}
	}

	std::shared_ptr<Object> Scene3D::CreateEmptyObject(const std::string& name, Object* parent)
	{
		if (parent != nullptr)
		{
			unsigned int parentID = parent->GetID();
			if (auto iter = m_Objects.find(parentID); iter == m_Objects.end())
			{
				BS_CORE_ERROR("Parent object not found!");
				return std::shared_ptr<Object>();
			}
			else if (iter->second->GetType() != Object::Type::Empty)
			{
				BS_CORE_ERROR("Parent object is not an EmptyObject!");
				return std::shared_ptr<Object>();
			}
			else if (parent != iter->second.get())
			{
				BS_CORE_ERROR("Parent object not found!");
				return std::shared_ptr<Object>();
			}
			else
			{
				unsigned int id = AssignID();
				std::shared_ptr<EmptyObject> emptyObject = std::make_shared<EmptyObject>(id, name, parent);
				m_Objects[id] = emptyObject;
				return emptyObject;
			}
		}
		else
		{
			unsigned int id = AssignID();
			std::shared_ptr<EmptyObject> emptyObject = std::make_shared<EmptyObject>(id, name, parent);
			m_Objects[id] = emptyObject;
			return emptyObject;
		}
	}
	std::shared_ptr<Object> Scene3D::CreatePoint2D(const std::string& name)
	{
		return nullptr;
	}
}
