#include "Program.h"
#include "WindowManager.h"

#include <stdexcept>
#include <string>

#include "Input.h"
#include "CoordinateManager.h"
#include "GridRenderer.h"
#include "GraphRenderer.h"
#include "Settings.h"
#include "ImGuiManager.h"

void error_callback(int error, const char* msg) {
	std::string s;
	s = " [" + std::to_string(error) + "] " + msg + '\n';
	MessageBoxA(0, s.c_str(), "GLFW error", MB_OK);
}

Program::Program()
{
	running = true;
	// init glfw
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
	{
		running = false;
		MessageBoxA(0, "GLFWInit != GLFW_TRUE", "GLFW error", MB_OK);
	}

	// create window
	glfwWindowHint(GLFW_SAMPLES, 8);
	WindowManager::AddWindow("main", new Window(800, 800, "Graph Drawer", WINDOWMODE::WINDOWED));
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	WindowManager::AddWindow("child", new Window(400, 600, "Graph Menu", WINDOWMODE::WINDOWED));
	glfwSetWindowPos(WindowManager::GetWindow("child")->GetHandle(), WindowManager::GetWindow("main")->GetPosX(), WindowManager::GetWindow("main")->GetPosY());
	WindowManager::GetWindow("main")->MakeContextCurrent();
	WindowManager::GetWindow("main")->SetVsync(1);
	WindowManager::GetWindow("child")->MakeContextCurrent();
	WindowManager::GetWindow("child")->SetVsync(1);
	ImGuiManager::Init();

	// set input window
	WindowManager::GetWindow("main")->MakeContextCurrent();
	Input::AddWindow(WindowManager::GetWindow("main")->GetHandle());

	// init glew
	if (glewInit() != GLEW_OK)
	{
		running = false;
		MessageBoxA(0, "GLEWInit != GLEW_OK", "GLEW error", MB_OK);
	}
	glEnable(GL_MULTISAMPLE);
	mainLoop();
}

Program::~Program()
{
	glfwSetErrorCallback(NULL);
	ImGuiManager::Exit();
	WindowManager::Terminate();
	glfwTerminate();
}

int Program::mainLoop()
{
	GraphRenderer::CreateGraph("x", 0.8f, 0.f, 0.5f);
	while (running)
	{
		// check if program should close
		if (glfwWindowShouldClose(WindowManager::GetWindow("main")->GetHandle()))
			running = false;

		// menu
		WindowManager::GetWindow("child")->MakeContextCurrent();
		ImGuiManager::CreateMenu();
		WindowManager::GetWindow("main")->MakeContextCurrent();

		// clear screen
		glClearColor(Settings::backgroundColor[0], Settings::backgroundColor[1], Settings::backgroundColor[2], 0);
		glClear(GL_COLOR_BUFFER_BIT);

		// update coordinates
		CoordinateManager::Update();

		// draw grid
		if (Settings::drawGrid)
		{
			GridRenderer::GenerateGrid();
			GridRenderer::RenderGrid();
		}

		// draw graphs
		GraphRenderer::GenerateGraphs();
		GraphRenderer::RenderGraphs();

		// swap buffers and poll input
		WindowManager::GetWindow("child")->MakeContextCurrent();
		ImGuiManager::Render();
		WindowManager::GetWindow("main")->Update();
		WindowManager::GetWindow("child")->Update();
		Input::Poll();
		glfwPollEvents();
	}
	return 0;
}
