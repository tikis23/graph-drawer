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
	double exp = log10(CoordinateManager::GetScaleLevel());
	int firstDigit = CoordinateManager::GetScaleLevel() / pow(10, floor(exp));
	for (int i = 2; i >= 0; i--)
	{
		if (firstDigit >= snapPoints[i])
		{
			firstDigit = snapPoints[i];
			break;
		}
	}
	double snap = powl(10, floor(exp) - 1) * firstDigit;

	// generate vertical vertices
	double x0 = CoordinateManager::ScreenToWorldX(0);
	double x1 = CoordinateManager::ScreenToWorldX(WindowManager::GetWindow("main")->GetWidth());
	x0 = floor(x0 / snap) - 1;
	x0 *= snap;
	for (double i = x0; i < x1; i += snap)
	{
		float pos = CoordinateManager::ScreenNormalizedX(CoordinateManager::WorldToScreenX(i));
		verticesVertical.push_back(pos);
	}

	// generate horizontal vertices
	double y0 = CoordinateManager::ScreenToWorldY(0);
	double y1 = CoordinateManager::ScreenToWorldY(WindowManager::GetWindow("main")->GetHeight());
	y0 = floor(y0 / snap) - 1;
	y0 *= snap;
	for (double i = y0; i < y1; i += snap)
	{
		float pos = CoordinateManager::ScreenNormalizedY(CoordinateManager::WorldToScreenY(i));
		verticesHorizontal.push_back(pos);
	}

	// generate thick lines at (0;0)
	// x
	verticesVertical.push_back(CoordinateManager::ScreenNormalizedX(CoordinateManager::WorldToScreenX(0)));
	verticesVertical.push_back(CoordinateManager::ScreenNormalizedX(CoordinateManager::WorldToScreenX(0) + 1));
	verticesVertical.push_back(CoordinateManager::ScreenNormalizedX(CoordinateManager::WorldToScreenX(0) - 1));
	// y
	verticesHorizontal.push_back(CoordinateManager::ScreenNormalizedY(CoordinateManager::WorldToScreenY(0)));
	verticesHorizontal.push_back(CoordinateManager::ScreenNormalizedY(CoordinateManager::WorldToScreenY(0) + 1));
	verticesHorizontal.push_back(CoordinateManager::ScreenNormalizedY(CoordinateManager::WorldToScreenY(0) - 1));
}

std::vector<float> GridRenderer::verticesVertical;
std::vector<float> GridRenderer::verticesHorizontal;