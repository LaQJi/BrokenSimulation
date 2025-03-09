#include "VoronoiMapping.h"

namespace Geometry
{
	VoronoiMapping::VoronoiMapping()
	{
	}

	VoronoiMapping::VoronoiMapping(const std::vector<Point<2>>& points)
	{
		this->points.reserve(points.size());
		for (const Point<2>&point : points)
		{
			this->points.push_back(std::make_shared<Point<2>>(point));
		}
		this->delaunay = std::make_shared<Delaunay<2>>(this->points);
	}

	VoronoiMapping::VoronoiMapping(const std::vector<std::shared_ptr<Point<2>>>& points)
	{
		this->points.reserve(points.size());
		for (const std::shared_ptr<Point<2>>&point : points)
		{
			this->points.push_back(point);
		}
		this->delaunay = std::make_shared<Delaunay<2>>(this->points);
	}

	bool VoronoiMapping::initialize()
	{
		if (!this->delaunay->initialize())
		{
			return false;
		}
		std::vector<std::shared_ptr<Simplex<2>>> simplices = this->delaunay->getSimplices();
		for (const std::shared_ptr<Simplex<2>>&simplex : simplices)
		{
			//std::shared_ptr<Point<2>> circumcenter = simplex->getCircumcenter();
			this->voronoiRegions.push_back(simplex);
		}
		return true;
	}

	std::vector<std::shared_ptr<Simplex<2>>> VoronoiMapping::getVoronoiRegions() const
	{
		return this->voronoiRegions;
	}
}
