#pragma once

#include <vector>

#include "ECS/Component.h"

namespace BrokenSim
{
	class VoronoiComponent : public Component
	{
	public:
		VoronoiComponent(Entity* entity, unsigned int numPoints = 6);

		void AddPoint();
		void AddPoint(const glm::vec2& point);
		void AddPoint(const glm::vec3& point);

		void AddPoints(unsigned int numPoints);

		void RemovePoint(unsigned int index);

		void ClearPoints();

		glm::vec3& GetPoint(unsigned int index);
		const glm::vec3& GetPoint(unsigned int index) const;
		std::vector<glm::vec3>& GetPoints();
		const std::vector<glm::vec3>& GetPoints() const;

		glm::vec3& GetColor(unsigned int index);
		const glm::vec3& GetColor(unsigned int index) const;

		void SetViewportSize(float width, float height);
		glm::vec2 GetViewportSize() const { return m_ViewportSize; }

		const unsigned int GetNumPoints() const { return m_Points.size(); }

	private:
		std::vector<std::pair<glm::vec3, glm::vec3>> m_Points;

		glm::vec2 m_ViewportSize = { 720.0f, 720.0f };
	};
}
