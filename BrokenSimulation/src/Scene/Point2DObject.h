#pragma once

#include "Renderer/VertexArray.h"
#include "Scene/Object.h"

namespace BrokenSim
{
	class Point2DObject : public Object
	{
	public:
		Point2DObject(unsigned int id, const std::string& name = "Point2D");
		virtual ~Point2DObject();

		virtual void OnUpdate(TimeStep ts, std::shared_ptr<Shader>& shader) override;
		virtual void OnRender(std::shared_ptr<Shader> shader) override;
		virtual void OnEvent(Event& e) override;

		void SetPosition(const glm::vec2& pos) { m_Pos = pos; }
		void SetColor(const glm::vec3& color) { m_Color = color; }

		glm::vec2& GetPointPosition();
		glm::vec3& GetColor();
		
	private:
		glm::vec2 m_Pos;
		glm::vec3 m_Color;
	};
}
