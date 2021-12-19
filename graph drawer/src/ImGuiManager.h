#pragma once

class ImGuiManager
{
public:
	static void Init();
	static void Exit();
	static void CreateMenu();
	static void Render();
private:
	ImGuiManager();
	static bool hovered;
	static int selected;
};