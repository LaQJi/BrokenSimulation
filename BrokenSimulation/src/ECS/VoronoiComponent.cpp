#include "bspch.h"
#include "ECS/VoronoiComponent.h"

#include "Core/Log.h"
#include "Utils/Utils.h"

namespace BrokenSim
{
	VoronoiComponent::VoronoiComponent(Entity* entity, unsigned int numPoints)
		: Component(entity)
	{
		for (unsigned int i = 0; i < numPoints; i++)
		{
			glm::vec3 pos = { RandomFloat(0.0f, 1.0f), RandomFloat(0.0f, 1.0f), 0.0f };
			glm::vec3 color = { RandomFloat(0.0f, 1.0f), RandomFloat(0.0f, 1.0f), RandomFloat(0.0f, 1.0f) };
			m_Points.push_back({ pos, color });
		}
	}

	void VoronoiComponent::AddPoint()
	{
		glm::vec3 pos = { RandomFloat(0.0f, 1.0f), RandomFloat(0.0f, 1.0f), 0.0f };
		glm::vec3 color = { RandomFloat(0.0f, 1.0f), RandomFloat(0.0f, 1.0f), RandomFloat(0.0f, 1.0f) };
		m_Points.push_back({ pos, color });
	}

	void VoronoiComponent::AddPoint(const glm::vec2& point)
	{
		glm::vec3 pos = { point.x, point.y, 0.0f };
		glm::vec3 color = { RandomFloat(0.0f, 1.0f), RandomFloat(0.0f, 1.0f), RandomFloat(0.0f, 1.0f) };
		m_Points.push_back({ pos, color });
	}

	void VoronoiComponent::AddPoint(const glm::vec3& point)
	{
		glm::vec3 color = { RandomFloat(0.0f, 1.0f), RandomFloat(0.0f, 1.0f), RandomFloat(0.0f, 1.0f) };
		m_Points.push_back({ point, color });
	}

	void VoronoiComponent::AddPoints(unsigned int numPoints)
	{
		for (unsigned int i = 0; i < numPoints; i++)
		{
			glm::vec3 pos = { RandomFloat(0.0f, 1.0f), RandomFloat(0.0f, 1.0f), 0.0f };
			glm::vec3 color = { RandomFloat(0.0f, 1.0f), RandomFloat(0.0f, 1.0f), RandomFloat(0.0f, 1.0f) };
			m_Points.push_back({ pos, color });
		}
	}

	void VoronoiComponent::RemovePoint(unsigned int index)
	{
		m_Points.erase(m_Points.begin() + index);
	}

	void VoronoiComponent::ClearPoints()
	{
		m_Points.clear();
	}

	glm::vec3& VoronoiComponent::GetPoint(unsigned int index)
	{
		if (index < m_Points.size())
		{
			return m_Points[index].first;
		}
		else
		{
			BS_CORE_ERROR("Index out of range!");
			return m_Points[0].first;
		}
	}

	const glm::vec3& VoronoiComponent::GetPoint(unsigned int index) const
	{
		if (index < m_Points.size())
		{
			return m_Points[index].first;
		}
		else
		{
			BS_CORE_ERROR("Index out of range!");
			return m_Points[0].first;
		}
	}

	std::vector<glm::vec3>& VoronoiComponent::GetPoints()
	{
		std::vector<glm::vec3> points;
		std::transform(
			m_Points.begin(), m_Points.end(),
			std::back_inserter(points),
			[](const std::pair<glm::vec3, glm::vec3>& p) {
				return p.first;
			});
		return points;
	}

	const std::vector<glm::vec3>& VoronoiComponent::GetPoints() const
	{
		std::vector<glm::vec3> points;
		std::transform(
			m_Points.begin(), m_Points.end(),
			std::back_inserter(points),
			[](const std::pair<glm::vec3, glm::vec3>& p) {
				return p.first;
			});
		return points;
	}

	glm::vec3& VoronoiComponent::GetColor(unsigned int index)
	{
		if (index < m_Points.size())
		{
			return m_Points[index].second;
		}
		else
		{
			BS_CORE_ERROR("Index out of range!");
			return m_Points[0].second;
		}
	}

	const glm::vec3& VoronoiComponent::GetColor(unsigned int index) const
	{
		if (index < m_Points.size())
		{
			return m_Points[index].second;
		}
		else
		{
			BS_CORE_ERROR("Index out of range!");
			return m_Points[0].second;
		}
	}

	void VoronoiComponent::SetViewportSize(float width, float height)
	{
		m_ViewportSize = { width, height };
	}
}
