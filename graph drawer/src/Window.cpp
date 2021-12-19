#include "Window.h"
#include <Windows.h>
#include "WindowManager.h"
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
	glfwSetFramebufferSizeCallback(handle, framebufferResizeCallback);
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

int Window::GetPosX() const
{
	int xpos, ypos;
	glfwGetWindowPos(handle, &xpos, &ypos);
	return xpos;
}

int Window::GetPosY() const
{
	int xpos, ypos;
	glfwGetWindowPos(handle, &xpos, &ypos);
	return ypos;
}

void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	Window* win = WindowManager::GetWindow(window);
	win->width = width;
	win->height = height;
	win->MakeContextCurrent();
	glViewport(0, 0, width, height);
}

