#include "GridRenderer.h"
#include <GL/glew.h>

#include "WindowManager.h"
#include "Input.h"
#include "CoordinateManager.h"
#include "Settings.h"
#include "FontRenderer.h"

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
	int eraseIndex = abs(floor(log10(snap)));

	// font variables
	float fontClipX = -1.0f + 2.0f / WindowManager::GetWindow("main")->GetWidth() * 20;
	float fontClipY = -1.0f + 2.0f / WindowManager::GetWindow("main")->GetHeight() * 20;

	// generate vertical vertices
	double x0 = CoordinateManager::ScreenToWorldX(0);
	double x1 = CoordinateManager::ScreenToWorldX(WindowManager::GetWindow("main")->GetWidth());
	x0 = floor(x0 / snap) - 1;
	x0 *= snap;
	for (double i = x0; i < x1; i += snap)
	{
		// calculate screen pos
		float pos = CoordinateManager::ScreenNormalizedX(CoordinateManager::WorldToScreenX(i));
		verticesVertical.push_back(pos);

		// get glyph
		if (!(i + snap * 0.1 > 0 && i - snap * 0.1 < 0))
		{
			std::string numString = std::to_string(i);
			int decimalIndex = numString.find('.');
			if (exp >= 1)
				numString.erase(numString.begin() + decimalIndex, numString.end());
			else
			{
				if(eraseIndex + decimalIndex + 1 < numString.size())
					numString.erase(numString.begin() + eraseIndex + decimalIndex + 1, numString.end());
			}
			if (pos > fontClipX)
				FontRenderer::AddToRenderQueue(numString, pos, -1);
		}
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

		// get glyph
		if (!(i + snap * 0.1 > 0 && i - snap * 0.1 < 0))
		{
			std::string numString = std::to_string(i);
			int decimalIndex = numString.find('.');
			if (exp >= 1)
				numString.erase(numString.begin() + decimalIndex, numString.end());
			else
			{
				if (eraseIndex + decimalIndex + 1 < numString.size())
					numString.erase(numString.begin() + eraseIndex + decimalIndex + 1, numString.end());
			}
			if (pos > fontClipX)
				FontRenderer::AddToRenderQueue(numString, -1, pos);
		}
	}

	// generate thick lines at (0;0)
	// x
	float posX = CoordinateManager::ScreenNormalizedX(CoordinateManager::WorldToScreenX(0) + 1);
	if (posX > -1 && posX < 1)
	{
		verticesVertical.push_back(CoordinateManager::ScreenNormalizedX(CoordinateManager::WorldToScreenX(0)));
		verticesVertical.push_back(posX);
		verticesVertical.push_back(CoordinateManager::ScreenNormalizedX(CoordinateManager::WorldToScreenX(0) - 1));
		if (posX > fontClipX)
			FontRenderer::AddToRenderQueue("0", posX, -1);
	}
	// y
	float posY = CoordinateManager::ScreenNormalizedY(CoordinateManager::WorldToScreenY(0) + 1);
	if (posY > -1 && posY < 1)
	{
		verticesHorizontal.push_back(CoordinateManager::ScreenNormalizedY(CoordinateManager::WorldToScreenY(0)));
		verticesHorizontal.push_back(CoordinateManager::ScreenNormalizedY(CoordinateManager::WorldToScreenY(0) + 1));
		verticesHorizontal.push_back(CoordinateManager::ScreenNormalizedY(CoordinateManager::WorldToScreenY(0) - 1));
		if (posY > fontClipY)
			FontRenderer::AddToRenderQueue("0", -1, posY);
	}
}

std::vector<float> GridRenderer::verticesVertical;
std::vector<float> GridRenderer::verticesHorizontal;