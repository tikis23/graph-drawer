#include "GraphRenderer.h"
#include <GL/glew.h>
#include "WindowManager.h"
#include "CoordinateManager.h"

void GraphRenderer::RenderGraphs()
{
	for (int i = 0; i < graphs.size(); i++)
	{
		if (!graphs[i].active)
			continue;
		glBegin(GL_LINE_STRIP);
		glColor3f(graphs[i].color[0], graphs[i].color[1], graphs[i].color[2]);
		for (int j = 0; j < graphs[i].points.size(); j+=2)
			glVertex2f(graphs[i].points[j], graphs[i].points[j+1]);
		glEnd();
	}
}

void GraphRenderer::GenerateGraphs()
{
	for (int i = 0; i < graphs.size(); i++)
	{
		if (!graphs[i].generate)
			continue;
		graphs[i].points.clear();
		for (int j = 0; j < WindowManager::GetWindow("main")->GetWidth(); j++)
		{
			long long int x = CoordinateManager::ScreenToWorldX(j);
			long long int y = x * x;
			float posx = CoordinateManager::ScreenNormalizedX(CoordinateManager::WorldToScreenX(x));
			float posy = CoordinateManager::ScreenNormalizedY(CoordinateManager::WorldToScreenY(y * 0.001 * 0 + 1000));
			graphs[i].points.push_back(posx);
			graphs[i].points.push_back(posy);
		}
		graphs[i].generate = false;
	}
}

void GraphRenderer::CreateGraph(std::string function, float r, float g, float b)
{
	Graph temp;
	temp.function = function;
	temp.color[0] = r;
	temp.color[1] = g;
	temp.color[2] = b;
	graphs.push_back(temp);
}

void GraphRenderer::RemoveGraph(int index)
{
	if (index < 0 || index > graphs.size() - 1)
		return;
	graphs.erase(graphs.begin() + index);
}

void GraphRenderer::ClearGraphs()
{
	graphs.clear();
}

std::vector<GraphRenderer::Graph>GraphRenderer::graphs;