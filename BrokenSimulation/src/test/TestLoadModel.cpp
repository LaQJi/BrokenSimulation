#include "TestLoadModel.h"

namespace test
{
	TestLoadModel::TestLoadModel()
	{
		inputModel = std::make_unique<Model>("res/models/ring_p.obj");
		shader = std::make_unique<Shader>("res/shaders/basic_v.shader", "res/shaders/basic_f.shader");
		shader->Bind();
	}

	TestLoadModel::~TestLoadModel()
	{
	}

	void TestLoadModel::OnUpdate(float deltaTime)
	{
	}

	void TestLoadModel::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		Renderer renderer;
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
			glm::mat4 view = glm::lookAt(viewPos, viewPos + viewFront, viewUp);
			glm::mat4 projection = glm::perspective(glm::radians(fov), 960.0f / 540.0f, 0.1f, 100.0f);
			glm::mat4 MVP = projection * view * model;

			normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));

			shader->Bind();
			shader->SetUniform3f("objectColor", objectColor);
			shader->SetUniform3f("lightColor", lightColor);
			shader->SetUniform3f("lightPos", lightPos);
			shader->SetUniform3f("viewPos", viewPos);
			shader->SetUniform1i("shininess", shininess);
			shader->SetUniformMat4f("u_MVP", MVP);
			shader->SetUniformMat4f("u_Model", model);
			shader->SetUniformMat3f("u_NormalMatrix", normalMatrix);

			renderer.Draw(*inputModel->va, *inputModel->ib, *shader);
		}
	}

	void TestLoadModel::OnImGuiRender()
	{
		ImGui::SliderFloat3("lightPos", &lightPos.x, -10.0f, 10.0f);
		ImGui::SliderFloat3("lightColor", &lightColor.x, 0.0f, 1.0f);
		ImGui::SliderFloat3("objectColor", &objectColor.x, 0.0f, 1.0f);
		ImGui::SliderFloat3("viewPos", &viewPos.x, -10.0f, 10.0f);
		ImGui::SliderFloat3("viewFront", &viewFront.x, -10.0f, 10.0f);
		ImGui::SliderFloat3("viewUp", &viewUp.x, -10.0f, 10.0f);
		ImGui::SliderFloat("fov", &fov, 0.0f, 180.0f);
		ImGui::SliderFloat("yaw", &yaw, -180.0f, 180.0f);
		ImGui::SliderFloat("pitch", &pitch, -180.0f, 180.0f);
		ImGui::SliderInt("shininess", &shininess, 0, 256);
	}
}
