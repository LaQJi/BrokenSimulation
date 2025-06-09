#pragma once

#include "ECS/ModelComponent.h"
#include "ECS/VoronoiComponent.h"

#include <glm/glm.hpp>


namespace BrokenSim
{
	namespace Geometry
	{
		// ����ģ�͸���3D Voronoiͼ�и�����ģ��
		std::vector<Meshes> cutModel(const ModelComponent& model, const VoronoiComponent& voronoi);

		// ��transform����ֽ�Ϊƽ�ơ���ת�����ŷ���
		bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);
	}
}
