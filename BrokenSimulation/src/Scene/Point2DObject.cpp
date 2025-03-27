#include "bspch.h"
#include "Scene/Point2DObject.h"

#include "Renderer/Renderer.h"
#include "Utils/Utils.h"

namespace BrokenSim
{
	Point2DObject::Point2DObject(unsigned int id, const std::string& name)
	{
		this->id = id;
		this->type = Type::Point2D;
		this->SetName(name);
		this->SetParent(nullptr);

		m_Pos = glm::vec2(RandomFloat(0.0f, 1.0f), RandomFloat(0.0f, 1.0f));
		m_Color = glm::vec4(RandomFloat(0.0f, 1.0f), RandomFloat(0.0f, 1.0f), RandomFloat(0.0f, 1.0f), 1.0f);
	}

	Point2DObject::~Point2DObject()
	{
	}

	void Point2DObject::OnUpdate(TimeStep ts, std::shared_ptr<Shader>& shader)
	{
	}

	void Point2DObject::OnRender(std::shared_ptr<Shader> shader)
	{

	}

	void Point2DObject::OnEvent(Event& e)
	{
	}

	glm::vec2& Point2DObject::GetPointPosition()
	{
		return m_Pos;
	}

	glm::vec3& Point2DObject::GetColor()
	{
		return m_Color;
	}
}
