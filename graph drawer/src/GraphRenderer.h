#pragma once
#include <vector>
#include <string>
class GraphRenderer
{
public:
	static void RenderGraphs();
	static void GenerateGraphs();
	static void CreateGraph(std::string function, float r, float g, float b);
	static void RemoveGraph(int index);
	static void ClearGraphs();
private:
	GraphRenderer();
	struct Graph
	{
		std::string function;
		std::vector<float>points;
		float color[3];
	};
	static std::vector<Graph>graphs;
};