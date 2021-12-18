#pragma once
#include "Window.h"
#include <string>
#include <map>
#include <GLFW/glfw3.h>
class WindowManager
{
public:
	static void AddWindow(std::string name, Window* window);
	static Window* GetWindow(std::string name);
	static Window* GetWindow(GLFWwindow* window);
	static void DeleteWindow(std::string name);
	static void Terminate();
private:
	WindowManager();

	static std::map<std::string, Window*> windows;
};