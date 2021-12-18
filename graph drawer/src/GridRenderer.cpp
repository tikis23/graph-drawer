#include "GridRenderer.h"
#include <GL/glew.h>

#include "WindowManager.h"
#include "Input.h"
#include "CoordinateManager.h"
#include "Settings.h"

void GridRenderer::RenderGrid()
{
	glBegin(GL_LINES);
	glColor3f(Settings::gridColor[0], Settings::gridColor[1], Settings::gridColor[2]);
	for (int i = 0; i < verticesVertical.size(); i++)
	{
		glVertex2f(verticesVertical[i], 1.0f);
		glVertex2f(verticesVertical[i], -1.0f);
	}
	for (int i = 0; i < verticesHorizontal.size(); i++)
	{
		glVertex2f(1.0f , verticesHorizontal[i]);
		glVertex2f(-1.0f, verticesHorizontal[i]);
	}										    
	glEnd();
}

void GridRenderer::GenerateGrid()
{
	verticesVertical.clear();
	verticesHorizontal.clear();

	// calculate snap point
	int snapPoints[] = { 1, 2, 5 };
	long long int exp = log10(CoordinateManager::GetScaleLevel());
	long long int firstDigit = CoordinateManager::GetScaleLevel() / powl(10, exp);
	for (int i = 2; i >= 0; i--)
	{
		if (firstDigit >= snapPoints[i])
		{
			firstDigit = snapPoints[i];
			break;
		}
	}
	long long int snap = powl(10, exp-1) * firstDigit;

	// generate vertical vertices
	long long int x0 = CoordinateManager::ScreenToWorldX(0);
	long long int x1 = CoordinateManager::ScreenToWorldX(WindowManager::GetWindow("main")->GetWidth());
	x0 = floor(x0 / snap) - 1;
	x0 *= snap;
	for (long long int i = x0; i < x1; i += snap)
	{
		float pos = CoordinateManager::ScreenNormalizedX(CoordinateManager::WorldToScreenX(i));
		verticesVertical.push_back(pos);
		if (i == 0)
		{
			pos = CoordinateManager::ScreenNormalizedX(CoordinateManager::WorldToScreenX(i)+1);
			verticesVertical.push_back(pos);
			pos = CoordinateManager::ScreenNormalizedX(CoordinateManager::WorldToScreenX(i)-1);
			verticesVertical.push_back(pos);
		}
	}

	// generate horizontal vertices
	long long int y0 = CoordinateManager::ScreenToWorldY(0);
	long long int y1 = CoordinateManager::ScreenToWorldY(WindowManager::GetWindow("main")->GetHeight());
	y0 = floor(y0 / snap) - 1;
	y0 *= snap;
	for (long long int i = y0; i < y1; i += snap)
	{
		float pos = CoordinateManager::ScreenNormalizedY(CoordinateManager::WorldToScreenY(i));
		verticesHorizontal.push_back(pos);
		if (i == 0)
		{
			pos = CoordinateManager::ScreenNormalizedY(CoordinateManager::WorldToScreenY(i)+1);
			verticesHorizontal.push_back(pos);
			pos = CoordinateManager::ScreenNormalizedY(CoordinateManager::WorldToScreenY(i)-1);
			verticesHorizontal.push_back(pos);
		}
	}
}

std::vector<float> GridRenderer::verticesVertical;
std::vector<float> GridRenderer::verticesHorizontal;