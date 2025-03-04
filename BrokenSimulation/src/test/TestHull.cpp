#include "TestHull.h"

namespace test
{
	TestHull::TestHull()
	{
		Timer timer;

		std::ifstream pointsFile("res/etc/delaunay.txt");
		std::string line;
		while (std::getline(pointsFile, line))
		{
			std::istringstream iss(line);
			double x, y, z;
			const std::size_t N = 3;
			if (!(iss >> x >> y >> z))
			{
				break;
			}
			std::array<double, N> coordinates = { x, y, z };
			Geometry::Point<N> p = Geometry::Point<3>(coordinates);
			points.push_back(std::make_shared<Geometry::Point<3>>(coordinates));
		}
		pointsFile.close();

		numPoints = points.size();

		std::ifstream hullsFile("res/etc/delaunay_simplex.txt");
		int pointIndex = 0;
		while (std::getline(hullsFile, line))
		{
			std::istringstream iss(line);
			std::string point, indexs;
			std::getline(iss, point, ',');
			std::getline(iss, indexs);

			std::istringstream issPoint(point);
			double x, y, z;
			if (!(issPoint >> x >> y >> z))
			{
				break;
			}
			std::array<double, 3> coordinates = { x, y, z };
			std::shared_ptr<Geometry::Point<3>> p = std::make_shared<Geometry::Point<3>>(coordinates);

			std::istringstream issIndexs(indexs);
			int index;
			while (issIndexs >> index)
			{
				pointSets[pointIndex].push_back(p);
			}
		}
		hullsFile.close();

		{
			Geometry::ConvexHull<3> hull = Geometry::ConvexHull<3>(points);
			hull.initialize();
			std::vector<std::shared_ptr<Geometry::Hyperplane<3>>> facets = hull.getFacets();
			std::ofstream hullFile("res/etc/hull.obj");
			std::map<std::shared_ptr<Geometry::Point<3>>, int> pointIndices;
			for (std::shared_ptr<Geometry::Point<3>> p : points)
			{
				hullFile << "v " << (*p)[0] << " " << (*p)[1] << " " << (*p)[2] << std::endl;
				pointIndices[p] = pointIndices.size();
			}
			for (std::shared_ptr<Geometry::Hyperplane<3>> facet : facets)
			{
				hullFile << "vn " << facet->getNormal()[0] << " " << facet->getNormal()[1] << " " << facet->getNormal()[2] << std::endl;
			}
			int index = 1;
			for (std::shared_ptr<Geometry::Hyperplane<3>> facet : facets)
			{
				hullFile << "f ";
				hullFile << pointIndices[(*facet)[0]] + 1 << "//" << index << " ";
				hullFile << pointIndices[(*facet)[1]] + 1 << "//" << index << " ";
				hullFile << pointIndices[(*facet)[2]] + 1 << "//" << index << std::endl;
				index++;
			}
			hullFile << std::endl;
			hullFile.close();
		}
		std::cout << "Time elapsed: " << timer.Elapsed() << "s" << std::endl;
	}

	TestHull::~TestHull()
	{
		std::cout << "TestHull destructor" << std::endl;
	}

	void TestHull::OnUpdate(float deltaTime)
	{
	}

	void TestHull::OnRender()
	{
	}

	void TestHull::OnImGuiRender()
	{
	}
}
