#include "Window.h"
#include <Windows.h>
Window::Window(int w_width, int w_height, std::string w_title, WINDOWMODE w_mode)
{
	width = w_width;
	height = w_height;
	title = w_title;
	mode = w_mode;
	const GLFWvidmode* monitorMode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	if (mode == WINDOWMODE::FULLSCREEN)
		handle = glfwCreateWindow(width, height, title.c_str(), glfwGetPrimaryMonitor(), NULL);
	else if (mode == WINDOWMODE::FULLSCREEN_WINDOWED)
		handle = glfwCreateWindow(monitorMode->width, monitorMode->height, title.c_str(), glfwGetPrimaryMonitor(), NULL);
	else if(mode == WINDOWMODE::WINDOWED)
		handle = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

	if (!handle)
		MessageBoxA(0, "Could not create window", "GLFW error", MB_OK);
}

Window::~Window()
{
	glfwDestroyWindow(handle);
}

GLFWwindow* Window::GetHandle() const
{
	return handle;
}

void Window::MakeContextCurrent()
{
	glfwMakeContextCurrent(handle);
}

void Window::Update()
{
	glfwSwapBuffers(handle);
	glfwPollEvents();
}

void Window::SetVsync(int val)
{
	glfwSwapInterval(val);
}

float Window::GetWidth() const
{
	return width;
}

float Window::GetHeight() const
{
	return height;
}

