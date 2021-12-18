#pragma once
#include <vector>
class GridRenderer
{
public:
	static void RenderGrid();
	static void GenerateGrid();
	static void SetColor(float r, float g, float b);
private:
	GridRenderer();

	static float color[3];
	static std::vector<float> verticesVertical;
	static std::vector<float> verticesHorizontal;
};