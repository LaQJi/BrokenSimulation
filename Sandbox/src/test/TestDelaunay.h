#pragma once

#include <memory>
#include <fstream>
#include <sstream>

#include "Renderer.h"
#include "Test.h"
#include "Geometry.h"
#include "Delaunay.h"
#include "util.h"
#include "Timer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test
{
	class TestDelaunay : public Test
	{
	public:
		TestDelaunay();
		~TestDelaunay();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		int numPoints = 0;
		std::vector<std::shared_ptr<Geometry::Point<2>>> points;
		std::unique_ptr<Geometry::Delaunay<2>> delaunay;
	};
}
