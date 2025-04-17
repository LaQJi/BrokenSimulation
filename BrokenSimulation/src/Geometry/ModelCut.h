#pragma once

#include "ECS/ModelComponent.h"
#include "ECS/VoronoiComponent.h"

namespace BrokenSim
{
	namespace Geometry
	{
		// 计算模型根据3D Voronoi图切割后的新模型
		std::vector<Meshes> cutModel(const ModelComponent& model, const VoronoiComponent& voronoi);
	}
}
