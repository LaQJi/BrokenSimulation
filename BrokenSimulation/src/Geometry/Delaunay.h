#pragma once

#include "Geometry/Geometry.h"

namespace BrokenSim
{
	namespace Geometry
	{
		template <std::size_t N>
		class Delaunay
		{
		public:
			// 构造函数
			Delaunay();
			Delaunay(const std::vector<Point<N>>& points);
			Delaunay(const std::vector<std::shared_ptr<Point<N>>>& points);


			// 成员函数
			bool initialize();
			std::vector<std::shared_ptr<Simplex<N>>> getSimplices() const;

		private:
			Point<N + 1> pointInfinity;
			std::vector<std::shared_ptr<Point<N + 1>>> points;
			std::shared_ptr<ConvexHull<N + 1>> convexHull;
			std::vector<std::shared_ptr<Simplex<N>>> simplices;
		};


		// Geometry functions
		template <std::size_t N>
		std::shared_ptr<Point<N + 1>> projectOntoParaboloid(const Point<N>& point);

		template <std::size_t N>
		std::shared_ptr<Point<N + 1>> projectOntoParaboloid(const std::shared_ptr<Point<N>>& point);

		template <std::size_t N>
		std::shared_ptr<Point<N>> projectOntoHyperplane(const Point<N + 1>& point);

		template <std::size_t N>
		std::shared_ptr<Point<N>> projectOntoHyperplane(const std::shared_ptr<Point<N + 1>>& point);
	}
}
