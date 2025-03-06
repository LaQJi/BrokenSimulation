#include "Delaunay.h"

namespace Geometry
{
	template <std::size_t N>
	Delaunay<N>::Delaunay()
	{
	}

	template <std::size_t N>
	Delaunay<N>::Delaunay(const std::vector<Point<N>>& points)
	{
		this->points.reserve(points.size());
		for (const Point<N>& point : points)
		{
			this->points.push_back(projectOntoParaboloid(point));
		}

		// 设置无穷远点
		this->pointInfinity = Point<N + 1>();
		this->pointInfinity[N] = -10.0;

		this->convexHull = std::make_shared<ConvexHull<N + 1>>(this->points);
	}

	template <std::size_t N>
	Delaunay<N>::Delaunay(const std::vector<std::shared_ptr<Point<N>>>& points)
	{
		this->points.reserve(points.size());
		for (const std::shared_ptr<Point<N>>& point : points)
		{
			this->points.push_back(projectOntoParaboloid(point));
		}

		// 设置无穷远点
		this->pointInfinity = Point<N + 1>();
		this->pointInfinity[N] = -10.0;

		this->convexHull = std::make_shared<ConvexHull<N + 1>>(this->points);
	}

	template <std::size_t N>
	bool Delaunay<N>::initialize()
	{
		if (!this->convexHull->initialize())
		{
			return false;
		}
		std::vector<std::shared_ptr<Hyperplane<N + 1>>> facets = this->convexHull->getFacets();
		std::vector<std::shared_ptr<Hyperplane<N>>> triangles;
		for (const std::shared_ptr<Hyperplane<N + 1>>&facet : facets)
		{
			Vector<N + 1> normal = facet->getNormal();
			if (normal * (this->pointInfinity - *((*facet)[0])) > 0.0)
			{
				std::array<std::shared_ptr<Point<N>>, N> triangleVertices;
				for (std::size_t i = 0; i < N; i++)
				{
					triangleVertices[i] = projectOntoHyperplane<N>((*facet)[i]);
				}
				triangles.push_back(std::make_shared<Hyperplane<N>>(triangleVertices));
			}
		}
		this->triangles = triangles;
		return true;
	}

	template <std::size_t N>
	std::vector<std::shared_ptr<Hyperplane<N>>> Delaunay<N>::getTriangles() const
	{
		return this->triangles;
	}

	// 显式实例化
	template class Delaunay<2>;
	template class Delaunay<3>;


	// Geometry functions
	template <std::size_t N>
	std::shared_ptr<Point<N + 1>> projectOntoParaboloid(const Point<N>& point)
	{
		std::shared_ptr<Point<N + 1>> projected = std::make_shared<Point<N + 1>>();
		for (std::size_t i = 0; i < N; i++)
		{
			(*projected)[i] = point[i];
		}
		(*projected)[N] = 0.0;
		for (std::size_t i = 0; i < N; i++)
		{
			(*projected)[N] += point[i] * point[i];
		}
		return projected;
	}

	template <std::size_t N>
	std::shared_ptr<Point<N + 1>> projectOntoParaboloid(const std::shared_ptr<Point<N>>& point)
	{
		std::shared_ptr<Point<N + 1>> projected = std::make_shared<Point<N + 1>>();
		for (std::size_t i = 0; i < N; i++)
		{
			(*projected)[i] = (*point)[i];
		}
		(*projected)[N] = 0.0;
		for (std::size_t i = 0; i < N; i++)
		{
			(*projected)[N] += (*point)[i] * (*point)[i];
		}
		return projected;
	}

	template <std::size_t N>
	std::shared_ptr<Point<N>> projectOntoHyperplane(const Point<N + 1>& point)
	{
		std::shared_ptr<Point<N>> projected = std::make_shared<Point<N>>();
		for (std::size_t i = 0; i < N; i++)
		{
			(*projected)[i] = point[i];
		}
		return projected;
	}

	template <std::size_t N>
	std::shared_ptr<Point<N>> projectOntoHyperplane(const std::shared_ptr<Point<N + 1>>& point)
	{
		std::shared_ptr<Point<N>> projected = std::make_shared<Point<N>>();
		for (std::size_t i = 0; i < N; i++)
		{
			(*projected)[i] = (*point)[i];
		}
		return projected;
	}

	// 显式实例化
	template std::shared_ptr<Point<3>> projectOntoParaboloid(const Point<2>& point);
	template std::shared_ptr<Point<4>> projectOntoParaboloid(const Point<3>& point);
	template std::shared_ptr<Point<2>> projectOntoHyperplane(const std::shared_ptr<Point<3>>& point);
	template std::shared_ptr<Point<3>> projectOntoHyperplane(const std::shared_ptr<Point<4>>& point);
}
