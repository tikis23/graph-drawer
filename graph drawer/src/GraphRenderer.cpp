#include "GraphRenderer.h"
#include <GL/glew.h>
#include "WindowManager.h"
#include "CoordinateManager.h"
void GraphRenderer::RenderGraphs()
{
	glBegin(GL_LINE_STRIP);
	glColor3f(1, 0, 0);
	for (int i = 0; i < WindowManager::GetWindow("main")->GetWidth(); i++)
	{
		long long int x = CoordinateManager::ScreenToWorldX(i);
		long long int y = x*x;
		float posx = CoordinateManager::ScreenNormalizedX(CoordinateManager::WorldToScreenX(x));
		float posy = CoordinateManager::ScreenNormalizedY(CoordinateManager::WorldToScreenY(y*0.001));
		glVertex2f(posx, posy);
	}
	glEnd();
}

void GraphRenderer::GenerateGraphs()
{

}

std::vector<GraphRenderer::Graph>GraphRenderer::graphs;