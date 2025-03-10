#pragma once

#include "Geometry/Geometry.h"
#include "Geometry/Delaunay.h"

namespace BrokenSim
{
	namespace Geometry
	{
		class VoronoiMapping
		{
		public:
			// ���캯��
			VoronoiMapping();
			VoronoiMapping(const std::vector<Point<2>>& points);
			VoronoiMapping(const std::vector<std::shared_ptr<Point<2>>>& points);

			// ��Ա����
			bool initialize();
			std::vector<std::shared_ptr<Simplex<2>>> getVoronoiRegions() const;

		private:
			std::vector<std::shared_ptr<Point<2>>> points;
			std::shared_ptr<Delaunay<2>> delaunay;
			std::vector<std::shared_ptr<Simplex<2>>> voronoiRegions;
		};
	}
}
