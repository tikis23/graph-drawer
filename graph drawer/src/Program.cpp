#include "Program.h"
#include "WindowManager.h"

#include <stdexcept>
#include <string>

#include "Input.h"
#include "CoordinateManager.h"
#include "GridRenderer.h"
#include "GraphRenderer.h"
#include "Settings.h"

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
	WindowManager::GetWindow("main")->MakeContextCurrent();
	WindowManager::GetWindow("main")->SetVsync(1);

	// set input window
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
	WindowManager::Terminate();
	glfwTerminate();
}

int Program::mainLoop()
{
	while (running)
	{
		// check if program should close
		if (glfwWindowShouldClose(WindowManager::GetWindow("main")->GetHandle()))
			running = false;

		// menu

		// clear screen
		glClearColor(Settings::backgroundColor[0], Settings::backgroundColor[1], Settings::backgroundColor[2], 0);
		glClear(GL_COLOR_BUFFER_BIT);

		// update coordinates
		CoordinateManager::Update();

		// draw grid
		GridRenderer::SetColor(Settings::gridColor[0], Settings::gridColor[1], Settings::gridColor[2]);
		GridRenderer::GenerateGrid();
		GridRenderer::RenderGrid();

		// draw graphs
		GraphRenderer::GenerateGraphs();
		GraphRenderer::RenderGraphs();

		// swap buffers and poll input
		Input::Poll();
		WindowManager::GetWindow("main")->Update();
	}
	return 0;
}
