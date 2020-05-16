#pragma once
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
class Window
{
public:
	GLFWwindow* glfwwin = nullptr;
	const size_t width = 800;
	const size_t height = 600;
	void InitUI()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(glfwwin, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	Window()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwwin = glfwCreateWindow(1280, 720, "Translation Visualizer", NULL, NULL);
		glfwMakeContextCurrent(glfwwin);
		glfwSwapInterval(0);
		glViewport(0, 0, width, height);
	}

	void Update()
	{

	}
	~Window()
	{
		delete glfwwin;
	}

	
};

