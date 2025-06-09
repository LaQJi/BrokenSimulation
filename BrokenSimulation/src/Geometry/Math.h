#pragma once

#include "ECS/ModelComponent.h"
#include "ECS/VoronoiComponent.h"

#include <glm/glm.hpp>


namespace BrokenSim
{
	namespace Geometry
	{
		// 计算模型根据3D Voronoi图切割后的新模型
		std::vector<Meshes> cutModel(const ModelComponent& model, const VoronoiComponent& voronoi);

		// 将transform矩阵分解为平移、旋转和缩放分量
		bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);
	}
}
