#pragma once
#include <vector>
class GridRenderer
{
public:
	static void RenderGrid();
	static void GenerateGrid();
private:
	GridRenderer();

	static std::vector<float> verticesVertical;
	static std::vector<float> verticesHorizontal;
};