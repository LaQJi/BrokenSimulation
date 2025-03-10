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
	class TestMultiModels : public Test
	{
	public:
		TestMultiModels();
		~TestMultiModels();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		int modelCount = 8;

		std::vector<std::unique_ptr<Model>> models;
		std::unique_ptr<Shader> shader;

		std::vector<glm::vec3> modelPositions;
		std::vector<glm::vec3> modelColors;

		glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
		glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

		glm::vec3 viewPos = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 viewFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 viewUp = glm::vec3(0.0f, 1.0f, 0.0f);

		glm::mat3 normalMatrix = glm::mat3(1.0f);

		float fov = 45.0f;
		float yaw = -90.0f;
		float pitch = 0.0f;

		int shininess = 32;
	};
}
