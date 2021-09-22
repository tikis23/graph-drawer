#pragma once

#include "Shader.h"
#include "Window.h"
#include <vector>
#include <string>
#include "e_eval/e_eval.h"


class Renderer
{
public:
	Renderer(Window* window);
	~Renderer();
	void InitShaders();
	void Draw();
	Shader* GetShader()const { return mainShader; };
private:
	Window* mainWindow = 0;
	Window* otherWindow = 0;
	Shader* mainShader = 0;
	Shader* gridShader = 0;

private:
	struct GraphVariable
	{
		std::string function;
		std::vector<e_token>tokens;
		std::vector<e_operand>operands;
		std::vector<e_operand>operands_copy;
		bool wrong;
		bool check_draw;
		int draw_size;
		bool update;
		bool in_progress;
		bool finished;
		float color_r;
		float color_g;
		float color_b;
		unsigned int VBO;
		std::vector<float> data;
	};
	std::vector<GraphVariable>graphs;
	float bg_color[3] = {0.01f, 0.04f, 0.15f};
	float grid_color[3] = { 0.2f, 0.2f, 0.2f };
	float font_color[3] = { 0.4f, 0.4f, 0.4f };
	float fontScale[2] = { 0.25f, 0.4f };
	bool draw_grid = true;
	int lines = 10;
	bool MSAA = true;
	bool multi_threading = true;
	bool lock_scale = true;
	float scale[2] = { 20.f, 20.f };
	float sensitivity[2] = { 1, 1 };
	float zoom_sensitivity[2] = { 1, 1 };
	int selected_graph = 0;
	int selected_tab = 0;

	float sensitivity_core[2] = { 1, 1 };
	float zoom_sensitivity_core[2] = {1, 1};
	int origin_buffer = 0;
	float scale_buffer[2] = { 0, 0 };
	unsigned int grid_VBO = 0;
	int origin_x = 0, origin_y = 0;
	std::pair<double, double>mouseLast;
private:

	void GetGraphData(GraphVariable* it);
	void imgui_graphs_menu();
	void imgui_settings_menu();


	int w_sizex = 0;
	int w_sizey = 0;
	int w_posx = 0;
	int w_posy = 0;

	static float zoom_scale[2];
	static int cp_x;
	static int cp_y;
	static int offset_cpx;
	static int offset_cpy;
	static int buttonEvent;
	static bool mouse_pressed;
	static bool mouse_held;
	static void cursor_position_callback(GLFWwindow* window, double x, double y);
	static void imgui_window_move(GLFWwindow* window, int button, int action, int mods);
	static void main_window_mouse_key(GLFWwindow* window, int button, int action, int mods);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};