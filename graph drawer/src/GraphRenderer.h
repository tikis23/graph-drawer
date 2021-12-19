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

	struct Graph
	{
		bool active = true;
		bool generate = true;
		std::string function = "";
		std::vector<float>points;
		float color[3] = {1, 1, 1};
	};
	static std::vector<Graph>graphs;
private:
	GraphRenderer();
};