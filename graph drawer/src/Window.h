#pragma once
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

#define WINDOW_WINDOWED   0
#define WINDOW_FULLSCREEN 1
#define WINDOW_BORDERLESS 2

class Window
{
public:
	Window(int width, int height, std::string title, int mode);
	~Window();

	void MakeCurrent();
	void SetVSync(int mode);
	void Update();
public:
	GLFWwindow* GetHandle() const { return handle; };
	void GetSize(int* width, int* height) const { glfwGetFramebufferSize(handle, width, height); };
	float GetAspectRatio()const { int width; int height; glfwGetFramebufferSize(handle, &width, &height); if (height != 0)return (float)width / height; else return 0; };
	bool IsInFocus()const { return glfwGetWindowAttrib(handle, GLFW_FOCUSED); };
private:
	static void Framebuffer_callback(GLFWwindow* window, int width, int height);

private:
	GLFWwindow* handle; 
	std::string title;
};