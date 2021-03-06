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
	SetStyle();
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
						ImU32 color = IM_COL32(90, 120, 120, 80);
						// hovered
						if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiHoveredFlags_AllowWhenDisabled))
						{
							hovered = true;
							color = IM_COL32(60, 120, 90, 180);
						}
						// selected
						else if (selected == i)
							color = IM_COL32(60, 120, 90, 140);
						// clicked
						if (hovered && ImGui::IsMouseDown(0))
							color = IM_COL32(60, 120, 90, 200);
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
					if (ImGui::Button("New Graph"))
					{
						float r = (float)(std::rand() % 255) / 255;
						float g = (float)(std::rand() % 255) / 255;
						float b = (float)(std::rand() % 255) / 255;
						std::string func = randomFunctions[std::rand() % 33];
						GraphRenderer::CreateGraph(func, r, g, b);
					}
					ImGui::EndChild();
				}

				// current graph operands
				if (ImGui::BeginChild("##Operands", ImVec2{ ImGui::GetIO().DisplaySize.x - 16, ImGui::GetIO().DisplaySize.y - 342 }, true))
				{
					if (selected >= 0 && selected < GraphRenderer::graphs.size())
					{
						ImGui::Columns(2);
						for (int i = 0; i < GraphRenderer::graphs[selected].functionOperands.size(); i++)
						{
							if (i == GraphRenderer::graphs[selected].functionOperands.size() / 2)
								ImGui::NextColumn();
							if (GraphRenderer::graphs[selected].functionOperands[i].symbol == 'x')
								continue;
							float temp = GraphRenderer::graphs[selected].functionOperands[i].value;
							if (ImGui::DragFloat(std::string({ GraphRenderer::graphs[selected].functionOperands[i].symbol }).c_str(), &temp, 0.001f))
							{
								GraphRenderer::graphs[selected].functionOperands[i].value = temp;
								GraphRenderer::graphs[selected].evaluate = true;
							}
						}
					}
					ImGui::EndChild();
				}
				ImGui::EndTabItem();
			}		
			// settings tab
			if(ImGui::BeginTabItem("Settings"))
			{
				ImGui::ColorEdit3("Background Color", Settings::backgroundColor, ImGuiColorEditFlags_NoOptions);
				ImGui::ColorEdit3("Grid Color", Settings::gridColor, ImGuiColorEditFlags_NoOptions);
				ImGui::ColorEdit3("Font Color", Settings::fontColor, ImGuiColorEditFlags_NoOptions);
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
				if (ImGui::BeginTabBar("##infoTabs"))
				{
					if (ImGui::BeginTabItem("Operators"))
					{
						// print out infoOperations
						for (int i = 0; i < 29; i++)
							ImGui::Selectable(infoOperations[i].c_str());
						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem("Controls"))
					{
						// print out infoControls
						for (int i = 0; i < 5; i++)
							ImGui::Selectable(infoControls[i].c_str());
						ImGui::EndTabItem();
					}
					ImGui::EndTabBar();
				}
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

void ImGuiManager::SetStyle()
{
	auto &style = ImGui::GetStyle();

	// frames
	style.FrameRounding = 4;
	style.FrameBorderSize = 1;
	style.ChildRounding = 4;
	
	// colors
	style.Colors[ImGuiCol_FrameBg]              = ImVec4(0.35f, 0.47f, 0.47f, 0.27f);
	style.Colors[ImGuiCol_FrameBgHovered]       = ImVec4(0.35f, 0.47f, 0.47f, 0.78f);
	style.Colors[ImGuiCol_FrameBgActive]        = ImVec4(0.35f, 0.47f, 0.47f, 0.39f);
	style.Colors[ImGuiCol_CheckMark]            = ImVec4(0.23f, 0.47f, 0.35f, 1.00f);
	style.Colors[ImGuiCol_Button]               = ImVec4(0.35f, 0.47f, 0.47f, 0.47f);
	style.Colors[ImGuiCol_ButtonHovered]        = ImVec4(0.35f, 0.47f, 0.47f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive]         = ImVec4(0.35f, 0.47f, 0.47f, 1.00f);
	style.Colors[ImGuiCol_Tab]                  = ImVec4(0.35f, 0.47f, 0.47f, 0.58f);
	style.Colors[ImGuiCol_TabHovered]           = ImVec4(0.35f, 0.47f, 0.47f, 1.00f);
	style.Colors[ImGuiCol_TabActive]            = ImVec4(0.35f, 0.47f, 0.47f, 0.86f);
	style.Colors[ImGuiCol_ScrollbarGrab]        = ImVec4(0.35f, 0.47f, 0.47f, 0.58f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.35f, 0.47f, 0.47f, 0.78f);
	style.Colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4(0.35f, 0.47f, 0.47f, 1.00f);	
	style.Colors[ImGuiCol_Header]               = ImVec4(0.35f, 0.47f, 0.47f, 0.58f);
	style.Colors[ImGuiCol_HeaderHovered]        = ImVec4(0.35f, 0.47f, 0.47f, 0.78f);
	style.Colors[ImGuiCol_HeaderActive]         = ImVec4(0.35f, 0.47f, 0.47f, 1.00f);
}

bool ImGuiManager::hovered = false;
int ImGuiManager::selected = -1;
std::string ImGuiManager::infoOperations[] =
{
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

std::string ImGuiManager::infoControls[] =
{

	"",
	"    Left-Click          | pan",
	"    Scroll-Wheel        | zoom",
	"    Right-Click         | point selection snapped",
	"    Right-Click + Shift | point selection free",
};

std::string ImGuiManager::randomFunctions[] =
{
	"x",
	"5x",
	"10x",
	"x+5",
	"x+10",
	"5x+5",
	"5x+10",
	"10x+5",
	"10x+10",
	"x^2+x",
	"x^2+x+5",
	"2x^2+x",
	"2x^2+x+5",
	"2x^2+3x+5",
	"10x^2+3x+5",
	"x^3",
	"5x^3",
	"2$x",
	"3$x",
	"sin(x)",
	"cos(x)",
	"tan(x)",
	"ctg(x)",
	"asin(x)",
	"acos(x)",
	"atan(x)",
	"actg(x)",
	"10lx",
	"10l(x^2)",
	"2lx+5",
	"7lx+x",
	"5l(sin(x))",
	"4l(3$x)"
};