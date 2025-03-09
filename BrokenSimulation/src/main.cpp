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
	// 初始化GLFW
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	// 创建窗口并创建OpenGL上下文
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(960, 540, "Broken Simulation", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	// 使OpenGL的上下文为当前窗口的上下文
	glfwMakeContextCurrent(window);

	// 初始化GLEW
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	{
		// 启用混合
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		Renderer renderer;

		// 启用深度测试和背面剔除
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glEnable(GL_CULL_FACE));

		// 设置ImGUI上下文
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // 启用键盘控制
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // 启用游戏手柄控制
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // 启用停靠
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // 启用多窗口
		io.ConfigViewportsNoAutoMerge = true;					  // 启用自动合并
		io.ConfigViewportsNoTaskBarIcon = false;				  // 显示任务栏图标

		// 设置ImGUI样式
		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// 设置ImGUI平台
		ImGui_ImplOpenGL3_Init("#version 130");
		ImGui_ImplGlfw_InitForOpenGL(window, true);

		// 创建测试菜单
		test::Test* currentTest;
		test::TestMenu* testMenu = new test::TestMenu(currentTest);
		currentTest = testMenu;

		testMenu->RegisterTest<test::TestHull>("Convex Hull");
		testMenu->RegisterTest<test::TestDelaunay>("Delaunay");
		testMenu->RegisterTest<test::TestLoadModel>("Load Model");

		while (!glfwWindowShouldClose(window))
		{
			// 清理颜色缓冲区和深度缓冲区
			renderer.Clear();
			
			// ImGUI新帧
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

			// ImGUI渲染
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			// 更新视口
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}

			// 交换缓冲区和轮询事件
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		delete currentTest;
		if (currentTest != testMenu)
			delete testMenu;
	}

	// 清理ImGUI
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// 清理GLFW
	glfwTerminate();

	return 0;
}