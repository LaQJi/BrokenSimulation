#pragma once

#include <memory>
#include <fstream>
#include <sstream>

#include "Renderer.h"
#include "Test.h"
#include "Geometry.h"
#include "util.h"
#include "Timer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test
{
	class TestHull : public Test
	{
	public:
		TestHull();
		~TestHull();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		int numPoints = 0;
		std::vector<std::shared_ptr<Geometry::Point<3>>> points;
		std::unordered_map<int, std::vector<std::shared_ptr<Geometry::Point<3>>>> pointSets;
		std::unordered_map<int, std::shared_ptr<Geometry::ConvexHull<3>>> convexHulls;
	};
}
