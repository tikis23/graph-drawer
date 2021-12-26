#include "PointSelector.h"

#include <GL/glew.h>
#include <vector>
#include <cmath>

#include "CoordinateManager.h"
#include "Settings.h"
#include "FontRenderer.h"
#include "Input.h"
#include "WindowManager.h"
#include "GraphRenderer.h"
#include "SEval/SEval.h"

void PointSelector::DrawSelectedPoint()
{
	// get variables
	int mouseX, mouseY;
	Input::GetMousePosition(WindowManager::GetWindow("main")->GetHandle(), mouseX, mouseY);
	double linePosWorldX = CoordinateManager::ScreenToWorldX(mouseX);
	double linePosWorldY = CoordinateManager::ScreenToWorldY(mouseY);

	// draw lines
	glBegin(GL_LINES);
	glColor3f(Settings::gridColor[0]*1.5f, Settings::gridColor[1]*1.5f, Settings::gridColor[2]*1.5f);
	// vertical
	glVertex2f(CoordinateManager::ScreenNormalizedX(mouseX), -1);
	glVertex2f(CoordinateManager::ScreenNormalizedX(mouseX), 1);
	glEnd();

	// calculate graph values at mouseX
	std::vector<std::pair<double, int>>values;
	for (int i = 0; i < GraphRenderer::graphs.size(); i++)
	{
		int xIndex = -1;
		for (int k = 0; k < GraphRenderer::graphs[i].functionOperands.size(); k++)
		{
			if (GraphRenderer::graphs[i].functionOperands[k].symbol == 'x')
			{
				xIndex = k;
				break;
			}
		}
		if (xIndex >= 0)
			GraphRenderer::graphs[i].functionOperands[xIndex].value = linePosWorldX;
		int error = 0;
		double val = SEval::evaluate(GraphRenderer::graphs[i].functionTokens, GraphRenderer::graphs[i].functionOperands, &error);
		if (error > 0 || !std::isfinite(val))
			continue;
		values.push_back(std::pair<double, int>(val, i));
	}
	// find closest to mouseY
	double dist = NAN;
	int closestValIndex = -1;
	if (!values.empty())
	{
		dist = abs(linePosWorldY - values[0].first);
		closestValIndex = 0;
	}
	for (int i = 0; i < values.size(); i++)
	{
		if (abs(linePosWorldY - values[i].first) < dist)
		{
			dist = abs(linePosWorldY - values[i].first);
			closestValIndex = i;
		}
	}

	if (closestValIndex != -1)
	{
		// draw point on graph
		glPointSize(10);
		glBegin(GL_POINTS);
		glColor3f(GraphRenderer::graphs[values[closestValIndex].second].color[0], GraphRenderer::graphs[values[closestValIndex].second].color[1],
			GraphRenderer::graphs[values[closestValIndex].second].color[2]);
		glVertex2f(CoordinateManager::ScreenNormalizedX(mouseX), CoordinateManager::ScreenNormalizedY(CoordinateManager::WorldToScreenY(values[closestValIndex].first)));
		glEnd();
		glPointSize(1);

		// draw box
		int widthPixels = 120;
		int heightPixels = 50;
		if (CoordinateManager::ScreenNormalizedX(mouseX) > 0)
			widthPixels = -widthPixels;	
		if (CoordinateManager::ScreenNormalizedY(CoordinateManager::WorldToScreenY(values[closestValIndex].first)) > 0)
			heightPixels = -heightPixels;

		float quadX0 = CoordinateManager::ScreenNormalizedX(mouseX);
		float quadX1 = CoordinateManager::ScreenNormalizedX(CoordinateManager::WorldToScreenX(linePosWorldX) + widthPixels);
		float quadY0 = CoordinateManager::ScreenNormalizedY(CoordinateManager::WorldToScreenY(values[closestValIndex].first));
		float quadY1 = CoordinateManager::ScreenNormalizedY(CoordinateManager::WorldToScreenY(values[closestValIndex].first) + heightPixels);

		glBegin(GL_QUADS);
		glColor3f(0.1, 0.1, 0.1);
		glVertex2f(quadX0, quadY0);
		glVertex2f(quadX1, quadY0);
		glVertex2f(quadX1, quadY1);
		glVertex2f(quadX0, quadY1);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glColor3f(0.5, 0.5, 0.5);
		glVertex2f(quadX0, quadY0);
		glVertex2f(quadX1, quadY0);
		glVertex2f(quadX1, quadY1);
		glVertex2f(quadX0, quadY1);
		glVertex2f(quadX0, quadY0);
		glEnd();

		// draw font
		if (widthPixels < 0)
			quadX0 = quadX1;
		if (heightPixels < 0)
			quadY0 = quadY1;
		quadY0 += CoordinateManager::ScreenNormalizedY(25) + 1;
		quadX0 += CoordinateManager::ScreenNormalizedX(5) + 1;

		// y
		FontRenderer::AddToRenderQueue(std::to_string(values[closestValIndex].first), quadX0, quadY0);
		// x
		FontRenderer::AddToRenderQueue(std::to_string(linePosWorldX), quadX0, quadY0 - CoordinateManager::ScreenNormalizedY(20) - 1);
	}
}

void PointSelector::DrawSelectedPointSnapped()
{
	DrawSelectedPoint();
}