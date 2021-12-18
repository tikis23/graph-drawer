#include "WindowManager.h"

void WindowManager::AddWindow(std::string name, Window* window)
{
	DeleteWindow(name);
	windows[name] = window;
}

Window* WindowManager::GetWindow(std::string name)
{
	if (windows.count(name) > 0)
		return windows.at(name);
	else
		return nullptr;
}

Window* WindowManager::GetWindow(GLFWwindow* window)
{
	for (auto& it : windows)
	{
		if (it.second->GetHandle() == window)
			return it.second;
	}
	return nullptr;
}

void WindowManager::DeleteWindow(std::string name)
{
	if (windows.count(name) > 0)
	{
		if(windows.at(name) != nullptr)
		delete windows.at(name);
		windows.erase(name);
	}
}

void WindowManager::Terminate()
{
	for (auto& it : windows)
	{
		if (it.second != nullptr)
			delete it.second;
	}
}

std::map<std::string, Window*> WindowManager::windows;