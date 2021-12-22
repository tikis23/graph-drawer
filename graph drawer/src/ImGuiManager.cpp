#include "ImGuiManager.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl2.h"
#include "imgui/imgui_stdlib.h"

#include "WindowManager.h"
#include "GraphRenderer.h"
#include "Settings.h"

void ImGuiManager::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(WindowManager::GetWindow("child")->GetHandle(), true);
	ImGui_ImplOpenGL2_Init();
	ImGui::GetIO().IniFilename = NULL;
}

void ImGuiManager::Exit()
{
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiManager::CreateMenu()
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	Window* win = WindowManager::GetWindow("child");
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
	ImGui::SetNextWindowPos({ 0, 0 });
	if (ImGui::Begin("Menu", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
	{
		if (ImGui::BeginTabBar("##menuTabBar"))
		{
			// graphs tab
			if(ImGui::BeginTabItem("Graphs"))
			{
				// graphs
				if(ImGui::BeginChild("##graphChildWindow", ImVec2{ ImGui::GetIO().DisplaySize.x - 16, ImGui::GetIO().DisplaySize.y - 300 }, true,
					ImGuiWindowFlags_AlwaysVerticalScrollbar))
				{
					for (int i = 0; i < GraphRenderer::graphs.size(); i++)
					{
						ImGui::PushID(i);

						// selectable
						bool hovered = false;
						bool notClicked = true;
						ImGui::Selectable("##hover", false, ImGuiSelectableFlags_AllowItemOverlap | ImGuiSelectableFlags_Disabled, { ImGui::GetWindowWidth() - 10, 20 });
						ImU32 color = IM_COL32(50, 50, 50, 255);
						if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiHoveredFlags_AllowWhenDisabled))
						{
							hovered = true;
							color *= 3;
						}
						else if (selected == i)
							color *= 2;
						if (hovered && ImGui::IsMouseDown(0))
							color *= 1.5f;
						float offset = ImGui::GetWindowPos().y + 5 + i * 20 + i * 9 - ImGui::GetScrollY();
						ImGui::GetWindowDrawList()->AddRectFilled({ ImGui::GetWindowPos().x + 5, offset}, { ImGui::GetWindowWidth() - 10, offset + 25}, color, 3);

						// active
						ImGui::SameLine(-1, 10);
						ImGui::Checkbox("##check", &GraphRenderer::graphs[i].active);
						if (ImGui::IsItemHovered())
							notClicked = false;

						// function input
						ImGui::SameLine();
						if (ImGui::InputText("##function", &GraphRenderer::graphs[i].function, ImGuiInputTextFlags_CallbackAlways))
						{
							GraphRenderer::graphs[i].generate = true;
						}

						// color
						ImGui::SameLine();
						ImGui::ColorEdit3("##color", &GraphRenderer::graphs[i].color[0], ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_DisplayRGB);
						if (ImGui::IsItemHovered())
							notClicked = false;

						// refresh
						ImGui::SameLine();
						if (ImGui::Button("r"))
							GraphRenderer::graphs[i].generate = true;
						if (ImGui::IsItemHovered())
							notClicked = false;

						// check if selected
						if (notClicked && hovered && ImGui::IsMouseClicked(0))
							selected = i;

						// remove
						ImGui::SameLine();
						if (ImGui::Button("X"))
						{
							GraphRenderer::RemoveGraph(i);
							selected = -1;
						}

						// padding
						ImGui::Dummy({ 0, 1 });

						ImGui::PopID();
					}
					if(ImGui::Button("New Graph"))
						GraphRenderer::CreateGraph("x*x", 0.5f, 0.5f, 0.5f);
					ImGui::EndChild();
				}
				// current graph settings
				{

				}
				ImGui::EndTabItem();
			}		
			// settings tab
			if(ImGui::BeginTabItem("Settings"))
			{
				ImGui::ColorEdit3("Background Color", Settings::backgroundColor, ImGuiColorEditFlags_NoOptions);
				ImGui::ColorEdit3("Grid Color", Settings::gridColor, ImGuiColorEditFlags_NoOptions);
				ImGui::Checkbox("Draw Grid", &Settings::drawGrid);
				ImGui::DragFloat("Scale", &Settings::globalScale, 0.01f);
				if (Settings::globalScale < 0.2f)
					Settings::globalScale = 0.2f;
				ImGui::DragFloat("Pan Speed", &Settings::panSpeed, 0.01f);
				ImGui::DragFloat("Zoom Speed", &Settings::zoomSpeed, 0.01f);
				ImGui::EndTabItem();
			}		
			// info tab
			if(ImGui::BeginTabItem("Info"))
			{
				// print out info
				for (int i = 0; i < 31; i++)
					ImGui::Selectable(info[i].c_str());
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}

		// window drag
		if (!ImGui::IsMouseDown(0))
		{
			if (ImGui::IsAnyItemHovered())
				hovered = true;
			else
				hovered = false;
		}
		else if(!hovered)
			glfwSetWindowPos(win->GetHandle(), win->GetPosX() + ImGui::GetMouseDragDelta(0).x, win->GetPosY() + ImGui::GetMouseDragDelta(0).y);

		ImGui::End();
	}
}

void ImGuiManager::Render()
{
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

bool ImGuiManager::hovered = false;
int ImGuiManager::selected = -1;
std::string ImGuiManager::info[] =
{
	"",
	"Currently supported operations",
	"",
	"Operators:", 
	"	a + b    | ",
	"	a - b    | ",
	"	a * b    | ",
	"	a / b    | ",
	"	a % b    | ",
	"	a $ b    | a - exponent ; b - number",
	"	a ^ b    | a - number   ; b - exponent",
	"	a l b    | a - log base ; b - number",
	"",
	"Functions:",
	"    sin(a)   | a - radians",
	"    asin(a)  | a - radians",
	"    cos(a)   | a - radians",
	"    acos(a)  | a - radians",
	"    tan(a)   | a - radians",
	"    atan(a)  | a - radians",
	"    ctg(a)   | a - radians",
	"    actg(a)  | a - radians",
	"    abs(a)   | |a|",
	"    dtr(a)   | a degrees to radians",
	"    rtd(a)   | a radians to degrees",
	"    floor(a) | ",
	"    ceil(a)  | ",
	"",
	"Constants:",
	"    pi - 3.14159265358979323846",
	"    e  - 2.71828182845904523536"
};