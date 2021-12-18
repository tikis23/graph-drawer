#pragma once
#include <vector>
#include <string>
class GraphRenderer
{
public:
	static void RenderGraphs();
	static void GenerateGraphs();
private:
	GraphRenderer();
	struct Graph
	{
		std::string function;
		std::vector<float>points;
	};
	static std::vector<Graph>graphs;
};