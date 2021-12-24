#include "GraphRenderer.h"
#include <GL/glew.h>
#include <cmath>
#include <algorithm>
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
	for (auto &graph : graphs)
	{
		if (graph.generate)
		{
			graph.error = 0;

			// convert string function to tokens
			graph.functionTokens = SEval::parseString(graph.function, &graph.error);
			if (graph.error > 0)
				continue;
			graph.functionOperands = SEval::getOperands(graph.functionTokens);
			for (int i = 0; i < graph.functionOperands.size(); i++)
				graph.functionOperands[i].value = 1;
			// sort operands in alphabetical order
			std::sort(graph.functionOperands.begin(), graph.functionOperands.end(), [](const SEval::operand& lhs, const SEval::operand& rhs)
				{
					return lhs.symbol < rhs.symbol;
				});
			graph.generate = false;
			graph.evaluate = true;
		}
		if (graph.evaluate)
		{
			// get index of x operand
			int xIndex = -1;
			for (int k = 0; k < graph.functionOperands.size(); k++)
			{
				if (graph.functionOperands[k].symbol == 'x')
				{
					xIndex = k;
					break;
				}
			}

			// generate points
			graph.points.clear();
			for (int j = 0; j < WindowManager::GetWindow("main")->GetWidth(); j++)
			{
				double x = CoordinateManager::ScreenToWorldX(j);
				if (xIndex >= 0)
					graph.functionOperands[xIndex].value = x;
				int error = 0;
				double y = SEval::evaluate(graph.functionTokens, graph.functionOperands, &error);
				if (error > 0 || !std::isfinite(y))
					continue;
				float posx = CoordinateManager::ScreenNormalizedX(j);
				float posy = CoordinateManager::ScreenNormalizedY(CoordinateManager::WorldToScreenY(y));
				graph.points.push_back(posx);
				graph.points.push_back(posy);
			}
			graph.evaluate = false;
		}
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