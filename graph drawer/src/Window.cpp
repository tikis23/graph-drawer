#include "Window.h"

Window::Window(int width, int height, std::string title, int mode)
{
    this->title = title;
	const GLFWvidmode* monitorMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	if(mode == WINDOW_FULLSCREEN)
		handle = glfwCreateWindow(width, height, title.c_str(), glfwGetPrimaryMonitor(), NULL);
	else if (mode == WINDOW_BORDERLESS)
		handle = glfwCreateWindow(monitorMode->width, monitorMode->height, title.c_str(), glfwGetPrimaryMonitor(), NULL);
	else if (mode == WINDOW_WINDOWED)
		handle = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    glfwSetFramebufferSizeCallback(handle, Framebuffer_callback);
}

Window::~Window()
{
	glfwDestroyWindow(handle);
}

void Window::MakeCurrent()
{
	glfwMakeContextCurrent(handle);
}

void Window::Update()
{
    glfwSwapBuffers(handle);
	glfwPollEvents();
}

void Window::SetVSync(int mode)
{
	glfwSwapInterval(mode);
}

void Window::Framebuffer_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
