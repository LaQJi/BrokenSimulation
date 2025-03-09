#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "util.h"

#include "test/Test.h"
#include "test/TestMenu.h"
#include "test/TestHull.h"
#include "test/TestDelaunay.h"
#include "test/TestLoadModel.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "glm/gtc/matrix_transform.hpp"

int main(void)
{
	// ��ʼ��GLFW
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	// �������ڲ�����OpenGL������
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(960, 540, "Broken Simulation", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	// ʹOpenGL��������Ϊ��ǰ���ڵ�������
	glfwMakeContextCurrent(window);

	// ��ʼ��GLEW
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	{
		// ���û��
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		Renderer renderer;

		// ������Ȳ��Ժͱ����޳�
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glEnable(GL_CULL_FACE));

		// ����ImGUI������
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // ���ü��̿���
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // ������Ϸ�ֱ�����
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // ����ͣ��
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // ���öര��
		io.ConfigViewportsNoAutoMerge = true;					  // �����Զ��ϲ�
		io.ConfigViewportsNoTaskBarIcon = false;				  // ��ʾ������ͼ��

		// ����ImGUI��ʽ
		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// ����ImGUIƽ̨
		ImGui_ImplOpenGL3_Init("#version 130");
		ImGui_ImplGlfw_InitForOpenGL(window, true);

		// �������Բ˵�
		test::Test* currentTest;
		test::TestMenu* testMenu = new test::TestMenu(currentTest);
		currentTest = testMenu;

		testMenu->RegisterTest<test::TestHull>("Convex Hull");
		testMenu->RegisterTest<test::TestDelaunay>("Delaunay");
		testMenu->RegisterTest<test::TestLoadModel>("Load Model");

		while (!glfwWindowShouldClose(window))
		{
			// ������ɫ����������Ȼ�����
			renderer.Clear();
			
			// ImGUI��֡
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			
			if (currentTest)
			{
				currentTest->OnUpdate(0.0f);
				currentTest->OnRender();
				ImGui::Begin("Test");
				if (currentTest != testMenu && ImGui::Button("<-"))
				{
					delete currentTest;
					currentTest = testMenu;
				}
				currentTest->OnImGuiRender();
				ImGui::End();
			}

			// ImGUI��Ⱦ
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			// �����ӿ�
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}

			// ��������������ѯ�¼�
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		delete currentTest;
		if (currentTest != testMenu)
			delete testMenu;
	}

	// ����ImGUI
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// ����GLFW
	glfwTerminate();

	return 0;
}