#pragma once
#include <string>
class ImGuiManager
{
public:
	static void Init();
	static void Exit();
	static void CreateMenu();
	static void Render();
	static void SetStyle();
private:
	ImGuiManager();
	static bool hovered;
	static int selected;
	static std::string infoOperations[];
	static std::string infoControls[];
};