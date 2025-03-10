#include "TestMultiModels.h"

namespace test
{
	TestMultiModels::TestMultiModels()
	{
		// 加载模型
		models.reserve(modelCount);
		modelPositions.reserve(modelCount);
		modelColors.reserve(modelCount);
		for (int i = 0; i < modelCount; i++)
		{
			std::stringstream ss;
			ss << "res/models/ring_" << i << ".obj";
			models.push_back(std::make_unique<Model>(ss.str()));
			modelPositions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			modelColors.push_back(glm::vec3(1.0f, 0.5f, 0.31f));
		}

		shader = std::make_unique<Shader>("res/shaders/basic_v.shader", "res/shaders/basic_f.shader");
		// 设置光照和环境属性
		shader->Bind();
	}

	TestMultiModels::~TestMultiModels()
	{
	}

	void TestMultiModels::OnUpdate(float deltaTime)
	{
	}

	void TestMultiModels::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		Renderer renderer;

		shader->Bind();
		shader->SetUniform3f("lightPos", lightPos);
		shader->SetUniform3f("lightColor", lightColor);
		shader->SetUniform3f("viewPos", viewPos);
		shader->SetUniform1i("shininess", shininess);

		glm::mat4 view = glm::lookAt(viewPos, viewPos + viewFront, viewUp);
		glm::mat4 projection = glm::perspective(glm::radians(fov), 960.0f / 540.0f, 0.1f, 100.0f);
		
		for (int i = 0; i < modelCount; i++)
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), modelPositions[i]);
			glm::mat4 MVP = projection * view * model;
			normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
			shader->SetUniformMat4f("u_MVP", MVP);
			shader->SetUniformMat4f("u_Model", model);
			shader->SetUniformMat3f("u_NormalMatrix", normalMatrix);
			shader->SetUniform3f("objectColor", modelColors[i]);
			renderer.DrawModel(*models[i], *shader);
		}
	}

	void TestMultiModels::OnImGuiRender()
	{
		ImGui::SliderFloat3("lightPos", &lightPos.x, -10.0f, 10.0f);
		ImGui::SliderFloat3("lightColor", &lightColor.x, 0.0f, 1.0f);
		ImGui::SliderFloat3("viewPos", &viewPos.x, -10.0f, 10.0f);
		ImGui::SliderFloat3("viewFront", &viewFront.x, -10.0f, 10.0f);
		ImGui::SliderFloat3("viewUp", &viewUp.x, -10.0f, 10.0f);
		ImGui::SliderFloat("fov", &fov, 0.0f, 180.0f);
		ImGui::SliderInt("shininess", &shininess, 0, 256);
		for (int i = 0; i < modelCount; i++)
		{
			std::stringstream ss;
			ss << "model " << i;
			ImGui::Text(ss.str().c_str());
			ImGui::PushID(i);
			ImGui::SliderFloat3("position", &modelPositions[i].x, -10.0f, 10.0f);
			ImGui::SliderFloat3("color", &modelColors[i].x, 0.0f, 1.0f);
			ImGui::PopID();
		}
	}
}
