#include "TestDelaunay.h"

namespace test
{
	TestDelaunay::TestDelaunay()
	{
		Timer timer;

		//std::ifstream pointsFile("res/etc/delaunay.txt");
		//std::string line;
		//while (std::getline(pointsFile, line))
		//{
		//	std::istringstream iss(line);
		//	double x, y, z;
		//	const std::size_t N = 3;
		//	if (!(iss >> x >> y >> z))
		//	{
		//		break;
		//	}
		//	std::array<double, N> coordinates = { x, y, z };
		//	points.push_back(std::make_shared<Geometry::Point<3>>(coordinates));
		//}

		//for (int i = 0; i < pow(2, 3); i++)
		//{
		//	std::array<double, 3> coordinates;
		//	for (int j = 0; j < 3; j++)
		//	{
		//		coordinates[2 - j] = (i & (1 << j)) ? 5 : -5;
		//	}
		//	points.push_back(std::make_shared<Geometry::Point<3>>(coordinates));
		//}


		//numPoints = points.size();

		//delaunay = std::make_unique<Geometry::Delaunay<3>>(points);

		//delaunay->initialize();
		//std::vector<std::shared_ptr<Geometry::Simplex<3>>> simplices = delaunay->getSimplices();

		//for (auto simplex : simplices)
		//{
		//	std::cout << "simplices:" << std::endl;
		//	for (int i = 0; i < 4; i++)
		//	{
		//		std::shared_ptr<Geometry::Point<3>> point = (*simplex)[i];
		//		std::cout << "(" << (*point)[0] << " " << (*point)[1] << " " << (*point)[2] << ") ";
		//	}
		//	std::cout << std::endl;
		//}

		std::ifstream pointsFile("res/etc/test2D.txt");
		std::string line;
		while (std::getline(pointsFile, line))
		{
			std::istringstream iss(line);
			double x, y;
			const std::size_t N = 2;
			if (!(iss >> x >> y))
			{
				break;
			}
			std::array<double, N> coordinates = { x, y };
			points.push_back(std::make_shared<Geometry::Point<2>>(coordinates));
		}

		for (int i = 0; i < pow(2, 2); i++)
		{
			std::array<double, 2> coordinates;
			for (int j = 0; j < 2; j++)
			{
				coordinates[1 - j] = (i & (1 << j)) ? 5 : -5;
			}
			points.push_back(std::make_shared<Geometry::Point<2>>(coordinates));
		}

		numPoints = points.size();

		delaunay = std::make_unique<Geometry::Delaunay<2>>(points);

		delaunay->initialize();
		std::vector<std::shared_ptr<Geometry::Simplex<2>>> simplices = delaunay->getSimplices();

		for (auto simplex : simplices)
		{
			std::cout << "simplices:" << std::endl;
			for (int i = 0; i < 3; i++)
			{
				std::shared_ptr<Geometry::Point<2>> point = (*simplex)[i];
				std::cout << "(" << (*point)[0] << " " << (*point)[1] << ") ";
			}
			std::cout << std::endl;
		}

		std::cout << "Delaunay test loaded in " << timer.Elapsed() << "s" << std::endl;
	}

	TestDelaunay::~TestDelaunay()
	{
	}

	void TestDelaunay::OnUpdate(float deltaTime)
	{
	}

	void TestDelaunay::OnRender()
	{
	}

	void TestDelaunay::OnImGuiRender()
	{
	}
}
