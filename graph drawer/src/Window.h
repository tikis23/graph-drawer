#pragma once
#include <GLFW/glfw3.h>
#include <string>

enum WINDOWMODE
{
	WINDOWED, FULLSCREEN, FULLSCREEN_WINDOWED
};

class Window
{
public:
	Window(int w_width, int w_height, std::string w_title, WINDOWMODE w_mode);
	~Window();
	GLFWwindow* GetHandle()const;
	void MakeContextCurrent();
	void Update();
	void SetVsync(int val);
	float GetWidth()const;
	float GetHeight()const;
	int GetPosX()const;
	int GetPosY()const;
private:
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

private:
	GLFWwindow* handle;
	int width, height;
	std::string title;
	WINDOWMODE mode;
};