#include "Renderer.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_stdlib.h"

#include <GLFW/glfw3.h>

#include <ctype.h>
#include <thread>

#include "FontRenderer.h"

// Theme generator
static ImVec4 base = ImVec4(0.502f, 0.075f, 0.256f, 1.0f);
static ImVec4 bg = ImVec4(0.200f, 0.220f, 0.270f, 1.0f);
static ImVec4 text = ImVec4(0.860f, 0.930f, 0.890f, 1.0f);
static float high_val = 0.8f;
static float mid_val = 0.5f;
static float low_val = 0.3f;
static float window_offset = -0.2f;
inline ImVec4 make_high(float alpha) {
	ImVec4 res(0, 0, 0, alpha);
	ImGui::ColorConvertRGBtoHSV(base.x, base.y, base.z, res.x, res.y, res.z);
	res.z = high_val;
	ImGui::ColorConvertHSVtoRGB(res.x, res.y, res.z, res.x, res.y, res.z);
	return res;
}
inline ImVec4 make_mid(float alpha) {
	ImVec4 res(0, 0, 0, alpha);
	ImGui::ColorConvertRGBtoHSV(base.x, base.y, base.z, res.x, res.y, res.z);
	res.z = mid_val;
	ImGui::ColorConvertHSVtoRGB(res.x, res.y, res.z, res.x, res.y, res.z);
	return res;
}
inline ImVec4 make_low(float alpha) {
	ImVec4 res(0, 0, 0, alpha);
	ImGui::ColorConvertRGBtoHSV(base.x, base.y, base.z, res.x, res.y, res.z);
	res.z = low_val;
	ImGui::ColorConvertHSVtoRGB(res.x, res.y, res.z, res.x, res.y, res.z);
	return res;
}
inline ImVec4 make_bg(float alpha, float offset = 0.f) {
	ImVec4 res(0, 0, 0, alpha);
	ImGui::ColorConvertRGBtoHSV(bg.x, bg.y, bg.z, res.x, res.y, res.z);
	res.z += offset;
	ImGui::ColorConvertHSVtoRGB(res.x, res.y, res.z, res.x, res.y, res.z);
	return res;
}
inline ImVec4 make_text(float alpha) {
	return ImVec4(text.x, text.y, text.z, alpha);
}
void theme_generator() 
{
	if(ImGui::ColorEdit3("base", (float*)&base, ImGuiColorEditFlags_PickerHueWheel));
	if(ImGui::ColorEdit3("bg", (float*)&bg, ImGuiColorEditFlags_PickerHueWheel));
	if(ImGui::ColorEdit3("text", (float*)&text, ImGuiColorEditFlags_PickerHueWheel));
	if(ImGui::SliderFloat("high", &high_val, 0, 1));
	if(ImGui::SliderFloat("mid", &mid_val, 0, 1));
	if(ImGui::SliderFloat("low", &low_val, 0, 1));
	if(ImGui::SliderFloat("window", &window_offset, -0.4f, 0.4f));

	ImGuiStyle& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_Text] = make_text(0.78f);
	style.Colors[ImGuiCol_TextDisabled] = make_text(0.28f);
	style.Colors[ImGuiCol_WindowBg] = make_bg(1.00f, window_offset);
	style.Colors[ImGuiCol_ChildBg] = make_bg(0.58f);
	style.Colors[ImGuiCol_PopupBg] = make_bg(0.9f);
	style.Colors[ImGuiCol_Border] = make_bg(0.6f, -0.05f);
	style.Colors[ImGuiCol_BorderShadow] = make_bg(0.0f, 0.0f);
	style.Colors[ImGuiCol_FrameBg] = make_bg(1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = make_mid(0.78f);
	style.Colors[ImGuiCol_FrameBgActive] = make_mid(1.00f);
	style.Colors[ImGuiCol_TitleBg] = make_low(1.00f);
	style.Colors[ImGuiCol_TitleBgActive] = make_high(1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = make_bg(0.75f);
	style.Colors[ImGuiCol_MenuBarBg] = make_bg(0.47f);
	style.Colors[ImGuiCol_ScrollbarBg] = make_bg(1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = make_low(1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = make_mid(0.78f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = make_mid(1.00f);
	style.Colors[ImGuiCol_CheckMark] = make_high(1.00f);
	style.Colors[ImGuiCol_SliderGrab] = make_bg(1.0f, .1f);
	style.Colors[ImGuiCol_SliderGrabActive] = make_high(1.0f);
	style.Colors[ImGuiCol_Button] = make_bg(1.0f, .2f);
	style.Colors[ImGuiCol_ButtonHovered] = make_mid(1.00f);
	style.Colors[ImGuiCol_ButtonActive] = make_high(1.00f);
	style.Colors[ImGuiCol_Header] = make_mid(0.76f);
	style.Colors[ImGuiCol_HeaderHovered] = make_mid(0.86f);
	style.Colors[ImGuiCol_HeaderActive] = make_high(1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
	style.Colors[ImGuiCol_ResizeGripHovered] = make_mid(0.78f);
	style.Colors[ImGuiCol_ResizeGripActive] = make_mid(1.00f);
	style.Colors[ImGuiCol_PlotLines] = make_text(0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = make_mid(1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = make_text(0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = make_mid(1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = make_mid(0.43f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = make_bg(0.73f);
	style.Colors[ImGuiCol_Tab] = make_bg(0.40f);
	style.Colors[ImGuiCol_TabHovered] = make_high(1.00f);
	style.Colors[ImGuiCol_TabActive] = make_mid(1.00f);
	style.Colors[ImGuiCol_TabUnfocused] = make_bg(0.40f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = make_bg(0.70f);
	style.Colors[ImGuiCol_DockingPreview] = make_high(0.30f);

	if (ImGui::Button("Export")) {
		ImGui::LogToTTY();
		ImGui::LogText("ImVec4* colors = ImGui::GetStyle().Colors;\n");
		for (int i = 0; i < ImGuiCol_COUNT; i++) {
			const ImVec4& col = style.Colors[i];
			const char* name = ImGui::GetStyleColorName(i);
			ImGui::LogText("colors[ImGuiCol_%s]%*s= ImVec4(%.2ff, %.2ff, %.2ff, %.2ff);\n",
				name, 23 - (int)strlen(name), "", col.x, col.y, col.z, col.w);
		}
		ImGui::LogFinish();
	}
}
void Theme() {
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(0.72f, 0.80f, 0.85f, 0.78f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.72f, 0.80f, 0.85f, 0.28f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.13f, 0.16f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.21f, 0.22f, 0.26f, 0.58f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.21f, 0.22f, 0.26f, 0.90f);
	colors[ImGuiCol_Border] = ImVec4(0.17f, 0.18f, 0.21f, 0.60f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.21f, 0.22f, 0.26f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.21f, 0.22f, 0.26f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.00f, 0.09f, 0.32f, 0.78f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.00f, 0.09f, 0.32f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.09f, 0.32f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.21f, 0.72f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.21f, 0.22f, 0.26f, 0.75f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.21f, 0.22f, 0.26f, 0.47f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.21f, 0.22f, 0.26f, 1.00f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.00f, 0.09f, 0.32f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.00f, 0.09f, 0.32f, 0.78f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.00f, 0.09f, 0.32f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 0.21f, 0.72f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.29f, 0.31f, 0.36f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 0.21f, 0.72f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.37f, 0.39f, 0.46f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.00f, 0.09f, 0.32f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.00f, 0.21f, 0.72f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.00f, 0.09f, 0.32f, 0.76f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.09f, 0.32f, 0.86f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.00f, 0.21f, 0.72f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.00f, 0.09f, 0.32f, 0.78f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.00f, 0.09f, 0.32f, 1.00f);
	colors[ImGuiCol_Tab] = ImVec4(0.21f, 0.22f, 0.26f, 0.40f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.00f, 0.21f, 0.72f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.00f, 0.09f, 0.32f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.21f, 0.22f, 0.26f, 0.40f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.21f, 0.22f, 0.26f, 0.70f);
	colors[ImGuiCol_DockingPreview] = ImVec4(0.00f, 0.21f, 0.72f, 0.30f);
	colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.72f, 0.80f, 0.85f, 0.63f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.00f, 0.09f, 0.32f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.72f, 0.80f, 0.85f, 0.63f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.00f, 0.09f, 0.32f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.09f, 0.32f, 0.43f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.21f, 0.22f, 0.26f, 0.73f);


}

std::string f_to_string(float value, int precision)
{
	std::string temp = std::to_string(value);
	for (int i = 0; i < temp.size(); i++)
	{
		if (temp[i] == '.')
		{
			if (i + precision < temp.size())
				temp.erase(temp.begin() + i + precision, temp.end());
		}
	}
	return temp;
}

Renderer::Renderer(Window* window)
{
	mainWindow = window;
	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_FALSE);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
	otherWindow = new Window(400, 500, "Graph Drawer Settings", WINDOW_WINDOWED);
	int x, y;
	glfwGetWindowPos(mainWindow->GetHandle(), &x, &y);
	glfwSetWindowPos(otherWindow->GetHandle(), x, y);
	glfwSetCursorPosCallback(otherWindow->GetHandle(), cursor_position_callback);
	glfwSetMouseButtonCallback(mainWindow->GetHandle(), main_window_mouse_key);
	glfwSetMouseButtonCallback(otherWindow->GetHandle(), imgui_window_move);
	glfwSetScrollCallback(mainWindow->GetHandle(), scroll_callback);
	glfwSetScrollCallback(otherWindow->GetHandle(), scroll_callback);
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(otherWindow->GetHandle(), true);
	ImGui_ImplOpenGL3_Init("#version 110");
	Theme();
	mouse_pressed = false;
	mouse_held = false;


}

Renderer::~Renderer()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glDeleteBuffersARB(1, &grid_VBO);
}

void Renderer::InitShaders()
{
	mainShader = new Shader;
	mainShader->LoadShaders("main", "res/shaders/main.vs", "res/shaders/main.fs");
	mainShader->Use();

	gridShader = new Shader;
	gridShader->LoadShaders("grid", "res/shaders/grid.vs", "res/shaders/grid.fs");
	glGenBuffersARB(1, &grid_VBO);
}

void Renderer::Draw()
{
	mainWindow->MakeCurrent();
	if(!glfwGetWindowAttrib(mainWindow->GetHandle(), GLFW_ICONIFIED))
		mainWindow->GetSize(&w_sizex, &w_sizey);

	glClearColor(bg_color[0], bg_color[1], bg_color[2], 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (MSAA)
		glEnable(GL_MULTISAMPLE);
	else
		glDisable(GL_MULTISAMPLE);

	// INPUT
	scale[0] += zoom_scale[0] * zoom_sensitivity_core[0];
	scale[1] += zoom_scale[1] * zoom_sensitivity_core[1];
	zoom_scale[0] = 0;
	zoom_scale[1] = 0;
	if (mouse_held)
	{
		double x, y;
		glfwGetCursorPos(mainWindow->GetHandle(), &x, &y);
		if (!mouse_pressed)
		{
			mouseLast = { x, y };
			mouse_pressed = true;
		}
		std::pair<double, double> mouse = { x, y };
		float xoffset = mouse.first - mouseLast.first;
		float yoffset = mouseLast.second - mouse.second;
		mouseLast = mouse;
		origin_x += (int)xoffset * sensitivity_core[0];
		origin_y += (int)yoffset * sensitivity_core[1];
	}
	else
		mouse_pressed = false;

	// SETTINGS WINDOW
	otherWindow->MakeCurrent();
	if (mainWindow->IsInFocus())
		glfwShowWindow(otherWindow->GetHandle());
	if (otherWindow->IsInFocus())
	{
		glfwShowWindow(mainWindow->GetHandle());
		glfwShowWindow(otherWindow->GetHandle());
	}
	if (buttonEvent == 1)
	{
		glfwGetWindowPos(otherWindow->GetHandle(), &w_posx, &w_posy);
		glfwSetWindowPos(otherWindow->GetHandle(), w_posx + offset_cpx, w_posy + offset_cpy);
		offset_cpx = 0;
		offset_cpy = 0;
		cp_x += offset_cpx;
		cp_y += offset_cpy;
	}

	// IMGUI
	if (!glfwGetWindowAttrib(otherWindow->GetHandle(), GLFW_ICONIFIED))
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
		if (ImGui::Begin("Menu", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize))
		{
			if (ImGui::BeginTabBar("Graph Drawer Menu"))
			{
				if (ImGui::BeginTabItem("Graphs"))
				{
					imgui_graphs_menu();
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Settings"))
				{
					imgui_settings_menu();
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Style"))
				{
					theme_generator();
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}

			ImGui::End();
			ImGui::PopStyleVar();
			ImGui::PopStyleVar();
			ImGui::PopStyleVar();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
		otherWindow->Update();
	}
	mainWindow->MakeCurrent();

	// RENDERING
	FontRenderer::SetScale(fontScale[0], fontScale[1]);

	// check for updates
	if (origin_x / w_sizex != origin_buffer || pow(10, ((int)scale[0] / 10)) != scale_buffer[0] || pow(10, ((int)scale[1] / 10)) != scale_buffer[1])
		for (auto& it : graphs)
			it.update = true;
	scale_buffer[0] = powf(10, ((int)scale[0] / 10));
	scale_buffer[1] = powf(10, ((int)scale[1] / 10));
	origin_buffer = origin_x / w_sizex;

	//grid
	gridShader->Use();
	float snaps[] = { 1, 2, 2.5 };
	int mx = snaps[0];
	int my = snaps[0];
	for (int i = 0; i < sizeof(snaps) / sizeof(int); i++)
	{
		if (scale[0] - ((int)scale[0] / 10) * 10 >= snaps[i])
			mx *= snaps[i];
		if (scale[1] - ((int)scale[1] / 10) * 10 >= snaps[i])
			my *= snaps[i];
	}
	int sx = powf(10, ((int)scale[0] / 10));
	int sy = powf(10, ((int)scale[1] / 10));
	if (draw_grid)
	{
		// vertical
		std::vector<float>data;
		std::vector<float>fatLine_data;

		float gx, x0 = 0, x1 = w_sizex * 6;
		gx = log10(x1 - x0);
		gx = floor(gx - 1); 
		gx = pow(10, gx);
		int normalized_origin_x = origin_x % w_sizex;
		normalized_origin_x *= (scale[0] - ((int)scale[0] / 10) * 10 + 1);
		if(gx > 0.5)
			for (float i = 0; i < x1; i += gx)
			{
				float pos = i * (scale[0] - ((int)scale[0] / 10) * 10 + 1) / mx + normalized_origin_x;
				if (i == 0 && origin_x < w_sizex && origin_x > -w_sizex)
				{
					fatLine_data.push_back(pos-3);
					fatLine_data.push_back(w_sizey);
					fatLine_data.push_back(pos+3);
					fatLine_data.push_back(w_sizey);
					fatLine_data.push_back(pos+3);
					fatLine_data.push_back(-w_sizey);
					fatLine_data.push_back(pos-3);
					fatLine_data.push_back(-w_sizey);
					FontRenderer::Draw("0.00", pos+3, -w_sizey, 10, font_color[0], font_color[1], font_color[2]);
				}
				else
				{
					float val = w_sizex * mx * (origin_x / w_sizex);

					data.push_back(pos);
					data.push_back(w_sizey);
					data.push_back(pos);
					data.push_back(-w_sizey);
					FontRenderer::Draw(f_to_string((i - val) / (mx * sx) , 3), pos, -w_sizey, 10, font_color[0], font_color[1], font_color[2]);

					pos = -i * (scale[0] - ((int)scale[0] / 10) * 10 + 1) / mx + normalized_origin_x;

					data.push_back(pos);
					data.push_back(w_sizey);
					data.push_back(pos);
					data.push_back(-w_sizey);
					if (pos > -w_sizex + 80)
						FontRenderer::Draw(f_to_string((-i - val) / (mx * sx), 3), pos, -w_sizey, 10, font_color[0], font_color[1], font_color[2]);
				}
			}
		if (data.size() > 0)
		{
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, grid_VBO);
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW_ARB);
			glVertexAttribPointerARB(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
			glEnableVertexAttribArrayARB(0);
			gridShader->setUniform2f("u_windowSize", w_sizex, w_sizey);
			gridShader->setUniform3f("u_color", grid_color[0], grid_color[1], grid_color[2]);
			glDrawArrays(GL_LINES, 0, data.size());
			if (fatLine_data.size() > 0)
			{
				glBufferDataARB(GL_ARRAY_BUFFER_ARB, fatLine_data.size() * sizeof(float), &fatLine_data[0], GL_STATIC_DRAW_ARB);
				glVertexAttribPointerARB(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
				glEnableVertexAttribArrayARB(0);
				gridShader->setUniform3f("u_color", grid_color[0] * 2.f, grid_color[1] * 2.f, grid_color[2] * 2.f);
				glDrawArrays(GL_QUADS, 0, fatLine_data.size());
			}
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
		}
		// horizontal
		data.clear();
		fatLine_data.clear();

		float gy, y0 = 0, y1 = w_sizey * 6;
		gy = log10(y1 - y0);
		gy = floor(gy - 1);
		gy = pow(10, gy);

		int normalized_origin_y = origin_y % w_sizey;
		normalized_origin_y *= (scale[1] - ((int)scale[1] / 10) * 10 + 1);
		if (gy > 0.5)
			for (float i = 0; i < y1; i += gy)
			{
				float pos = i * (scale[1] - ((int)scale[1] / 10) * 10 + 1) / my + normalized_origin_y;
				if (i == 0 && origin_y < w_sizey && origin_y > -w_sizey)
				{
					fatLine_data.push_back(w_sizex);
					fatLine_data.push_back(pos - 3);
					fatLine_data.push_back(w_sizex);
					fatLine_data.push_back(pos + 3);
					fatLine_data.push_back(-w_sizex);
					fatLine_data.push_back(pos + 3);
					fatLine_data.push_back(-w_sizex);
					fatLine_data.push_back(pos - 3);
					FontRenderer::Draw("0.00", -w_sizex, pos + 3, 10, font_color[0], font_color[1], font_color[2]);
				}
				else
				{
					float val = w_sizey * my * (origin_y / w_sizey);
					data.push_back(w_sizex);
					data.push_back(pos);
					data.push_back(-w_sizex);
					data.push_back(pos);
					FontRenderer::Draw(f_to_string((i - val) / (my * sy), 3), -w_sizex, pos, 10, font_color[0], font_color[1], font_color[2]);

					pos = -i * (scale[1] - ((int)scale[1] / 10) * 10 + 1) / my + normalized_origin_y;

					data.push_back(w_sizex);
					data.push_back(pos);
					data.push_back(-w_sizex);
					data.push_back(pos);
					if (pos > -w_sizey + 80)
						FontRenderer::Draw(f_to_string((-i - val) / (my * sy), 3), -w_sizex, pos, 10, font_color[0], font_color[1], font_color[2]);
				}
			}
		if (data.size() > 0)
		{
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, grid_VBO);
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW_ARB);
			glVertexAttribPointerARB(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
			gridShader->setUniform2f("u_windowSize", w_sizex, w_sizey);
			gridShader->setUniform3f("u_color", grid_color[0], grid_color[1], grid_color[2]);
			glDrawArrays(GL_LINES, 0, data.size());
			if (fatLine_data.size() > 0)
			{
				glBufferDataARB(GL_ARRAY_BUFFER_ARB, fatLine_data.size() * sizeof(float), &fatLine_data[0], GL_STATIC_DRAW_ARB);
				glVertexAttribPointerARB(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
				glEnableVertexAttribArrayARB(0);
				gridShader->setUniform3f("u_color", grid_color[0] * 2.f, grid_color[1] * 2.f, grid_color[2] * 2.f);
				glDrawArrays(GL_QUADS, 0, fatLine_data.size());
			}
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
		}
	}
	
	if (scale[0] - (int)scale[0] / 10 * 10 > 1.0f)
		sensitivity_core[0] = sensitivity[0] / (scale[0] - (int)scale[0] / 10 * 10);
	else
		sensitivity_core[0] = sensitivity[0];
	if (scale[1] - (int)scale[1] / 10 * 10 > 1.0f)
		sensitivity_core[1] = sensitivity[1] / (scale[1] - (int)scale[1] / 10 * 10);
	else
		sensitivity_core[1] = sensitivity[1];

	zoom_sensitivity_core[0] = zoom_sensitivity[0] + (scale[0] - (int)scale[0] / 10 * 10);
	zoom_sensitivity_core[1] = zoom_sensitivity[1] + (scale[1] - (int)scale[1] / 10 * 10);

	// graphs
	mainShader->Use();
	for (auto& it : graphs)
	{
		if (it.update)
		{
			if (it.in_progress == false)
			{
				it.in_progress = true;
				if (multi_threading)
					std::thread(&Renderer::GetGraphData, this, &it).detach();
				else
					GetGraphData(&it);
			}
			if (it.data.size() > 5 && it.finished)
			{
				it.data.erase(it.data.begin(), it.data.begin() + 2);
				it.data.pop_back();
				it.data.pop_back();
				glBindBufferARB(GL_ARRAY_BUFFER_ARB, it.VBO);
				glBufferDataARB(GL_ARRAY_BUFFER_ARB, it.data.size() * sizeof(float), &it.data[0], GL_STATIC_DRAW_ARB);
				it.draw_size = it.data.size();
				it.data.clear();
				it.update = false;
				it.in_progress = false;
				it.finished = false;
			}
		}
		if (it.check_draw && it.draw_size > 0)
		{
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, it.VBO);
			glVertexAttribPointerARB(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
			glEnableVertexAttribArrayARB(0);
			mainShader->setUniform2f("u_windowSize", w_sizex, w_sizey);
			mainShader->setUniform2f("u_origin", (float)origin_x * (scale[0] - ((int)scale[0] / 10) * 10 + 1), (float)origin_y * (scale[0] - ((int)scale[0] / 10) * 10 + 1));

			float mult_x = (scale[0] - ((int)scale[0] / 10) * 10 + 1) * sx;
			float mult_y = (scale[1] - ((int)scale[1] / 10) * 10 + 1) * sy;
			mainShader->setUniform2f("u_scale", mult_x, mult_y);
			mainShader->setUniform3f("u_color", it.color_r, it.color_g, it.color_b);
			glDrawArrays(GL_LINES, 0, it.draw_size);
		}
	}
	FontRenderer::Render(w_sizex, w_sizey);
}

void Renderer::GetGraphData(GraphVariable* it)
{
	double time = glfwGetTime();
	std::vector<e_token> tokens = it->tokens;
	std::vector<e_operand> operands = it->operands;
	it->data.clear();
	if (tokens.empty())
	{
		it->in_progress = false;
		return;
	}
	int op_index = 0;
	if (operands.size() != 0)
		for(int index = 0; index < operands.size(); index++)
			if (operands[index].operand == "x")
			{
				op_index = index;
				break;
			}

	float mult_x = pow(10, scale[0] / 10);
	float pos = w_sizex * 4;
	for (float i = (-pos - origin_x) / mult_x; i < (pos - origin_x) / mult_x; i += 1 / mult_x)
	{
		if (glfwGetTime() >= time + 5)
			break;
		int errorcheck = 0;
		if (operands.size() != 0)
			operands[op_index].value = i;
		float val = e_evaluate(tokens, operands, errorcheck);
		if (errorcheck == 0)
		{
			it->data.push_back(i);
			it->data.push_back(val);
			it->data.push_back(i);
			it->data.push_back(val);
			it->wrong = false;
		}
		else
		{
			it->wrong = true;
			it->in_progress = false;
			return;
		}
	}
	it->finished = true;
}

void Renderer::imgui_graphs_menu()
{
	// Graphs
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	if (ImGui::BeginChild("graphMenu", ImVec2(viewport->WorkSize.x - 15, viewport->WorkSize.x / 2), true, ImGuiWindowFlags_AlwaysVerticalScrollbar))
	{
		int tempLoopCounter = 0;
		for (auto& it : graphs)
		{
			ImGui::PushID(tempLoopCounter);
			bool selected = 0;
			if (tempLoopCounter == selected_graph)
				selected = 1;
			if (ImGui::Selectable("##hover", &selected, ImGuiSelectableFlags_AllowItemOverlap, ImVec2(viewport->WorkSize.x - 45, 20)))
			{
				if (selected_graph == tempLoopCounter)
					selected_graph = -1;
				else
					selected_graph = tempLoopCounter;
			}

			ImGui::SameLine(6);
			if (ImGui::Checkbox("##isActive", &it.check_draw))
				if (selected_graph == tempLoopCounter)
					selected_graph = -1;
				else
					selected_graph = tempLoopCounter;
			ImGui::SameLine();
			ImGui::PushItemWidth(120);
			if (it.wrong)
				ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 40, 130, 255));
			else
				ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(40, 255, 130, 255));

			if (ImGui::InputText("##Function", &it.function, ImGuiInputTextFlags_AutoSelectAll))
			{
				if (selected_graph == tempLoopCounter)
					selected_graph = -1;
				else
					selected_graph = tempLoopCounter;
				int error;
				it.tokens = e_compile(it.function, error);
				it.operands = e_get_operands(it.tokens);
				it.operands_copy = it.operands;
				if (error == 0)
					it.wrong = false;
				else
					it.wrong = true;
				it.update = true;
			}
			ImGui::PopStyleColor();
			ImGui::PopItemWidth();
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10);
			if (ImGui::Button("x"))
			{
				if (selected_graph == tempLoopCounter)
					selected_graph = -1;
				else if(selected_graph > tempLoopCounter)
					selected_graph--;
				glDeleteBuffersARB(1, &graphs[tempLoopCounter].VBO);
				graphs.erase(graphs.begin() + tempLoopCounter);
			}
			ImGui::Dummy(ImVec2(0.0f, 0.1f));
			ImGui::PopStyleColor();
			ImGui::PopStyleVar();
			ImGui::PopID();
			tempLoopCounter++;
		}
		ImVec4* colors = ImGui::GetStyle().Colors;
		ImGui::PushStyleColor(ImGuiCol_Text, colors[ImGuiCol_SliderGrabActive]);
		if (ImGui::Button("Add Graph"))
		{
			GraphVariable tempPush;
			tempPush.function = "x^2";
			tempPush.wrong = false;
			tempPush.check_draw = true;
			tempPush.draw_size = 0;
			tempPush.update = true;
			tempPush.in_progress = false;
			tempPush.finished = false;
			tempPush.color_r = 1;
			tempPush.color_g = 1;
			tempPush.color_b = 1;

			int error;
			tempPush.tokens = e_compile(tempPush.function, error);
			tempPush.operands = e_get_operands(tempPush.tokens);
			tempPush.operands_copy = tempPush.operands;
			glGenBuffersARB(1, &tempPush.VBO);
			graphs.push_back(tempPush);
		}
		ImGui::PopStyleColor();
		ImGui::EndChild();
	}
	if (!graphs.empty() && selected_graph >= 0 && selected_graph < graphs.size())
	{
		auto& it = graphs[selected_graph];
		if (ImGui::Button("Update Graph"))
			it.update = true;
		float tempColor[3] = { it.color_r, it.color_g, it.color_b };
		if (ImGui::ColorEdit3("Graph Color", tempColor, ImGuiColorEditFlags_Float));
		it.color_r = tempColor[0];
		it.color_g = tempColor[1];
		it.color_b = tempColor[2];
		ImGui::Text("");
		// operands
		ImGui::Text("Operands");
		if (ImGui::BeginChild("OperandsMenu", ImVec2(viewport->WorkSize.x - 15, viewport->WorkSize.x / 2 - 27), true))
		{
			if (!it.operands.empty() && !it.wrong)
				for (int i = 0; i < it.operands.size(); i++)
				{
					if (it.operands[i].operand != "x")
					{
						ImGui::PushItemWidth(120);
						float temp = it.operands[i].value;
						ImGui::DragFloat(it.operands[i].operand.c_str(), &temp, 0.01f);
						it.operands[i].value = temp;
						if (it.operands[i].value != it.operands_copy[i].value)
							it.update = true;
						it.operands_copy[i].value = it.operands[i].value;
						ImGui::PopItemWidth();
					}
				}
			ImGui::EndChild();
		}
	}
}

void Renderer::imgui_settings_menu()
{
	// other
	if (ImGui::ColorEdit3("Background Color", bg_color, ImGuiColorEditFlags_Float));
	if (ImGui::ColorEdit3("Grid Color", grid_color, ImGuiColorEditFlags_Float));
	if (ImGui::ColorEdit3("Font Color", font_color, ImGuiColorEditFlags_Float));
	if (ImGui::DragFloat2("Font Scale", fontScale, 0.001f, -100, 100));
	if (ImGui::DragFloat("Zoom", &scale[0], 0.0025f))
		scale[1] = scale[0];
	if (ImGui::DragFloat("Zoom Speed", &zoom_sensitivity[0], 0.001f, -100, 100));
	if (ImGui::DragFloat("Sensitivity", &sensitivity[0], 0.001f, -100, 100));
	zoom_sensitivity[1] = zoom_sensitivity[0];
	sensitivity[1] = sensitivity[0];
	if (ImGui::Checkbox("Grid", &draw_grid));
	if (ImGui::Checkbox("MSAA", &MSAA));
	if (ImGui::Checkbox("Multithreading", &multi_threading));
	ImGui::Dummy(ImVec2(0.0f, 20.f));
	if (ImGui::Button("Reset Origin"))
	{
		origin_x = 0;
		origin_y = 0;
	}
	if (ImGui::Button("Reset Zoom"))
	{
		scale[0] = 20;
		scale[1] = 20;
	}
}

void Renderer::cursor_position_callback(GLFWwindow* window, double x, double y)
{
	if (buttonEvent == 1)
	{
		offset_cpx =x - cp_x;
		offset_cpy =y - cp_y;
	}
}

void Renderer::imgui_window_move(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		if (!(ImGui::IsAnyItemFocused() || ImGui::IsAnyItemHovered()))
		{
			buttonEvent = 1;
			double x, y;
			glfwGetCursorPos(window, &x, &y);
			cp_x = floor(x);
			cp_y = floor(y);
		}
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		buttonEvent = 0;
		cp_x = 0;
		cp_y = 0;
	}
}

void Renderer::main_window_mouse_key(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		mouse_held = true;
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		mouse_held = false;
	}
}

void Renderer::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
		zoom_scale[0] += yoffset / 10;
	else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))
		zoom_scale[1] += yoffset / 10;
	else
	{
		zoom_scale[0] += yoffset / 10;
		zoom_scale[1] += yoffset / 10;
	}
}

int Renderer::cp_x;
int Renderer::cp_y;
int Renderer::offset_cpx;
int Renderer::offset_cpy;
int Renderer::buttonEvent;
bool Renderer::mouse_pressed;
bool Renderer::mouse_held;
float Renderer::zoom_scale[2];