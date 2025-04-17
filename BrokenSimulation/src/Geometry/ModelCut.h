#pragma once

#include "ECS/ModelComponent.h"
#include "ECS/VoronoiComponent.h"

namespace BrokenSim
{
	namespace Geometry
	{
		// ����ģ�͸���3D Voronoiͼ�и�����ģ��
		std::vector<Meshes> cutModel(const ModelComponent& model, const VoronoiComponent& voronoi);
	}
}
