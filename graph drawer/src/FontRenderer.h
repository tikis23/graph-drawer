#pragma once

#include "Shader.h"
#include <vector>
#include <string>

struct fontData
{
	float position[4];
	float color[3];
};

class FontRenderer
{
public:
	static void Setup(Shader* switchBackShader);
	static void Exit();
	static void Draw(std::string value, int x, int y, int offset, float color_r, float color_g, float color_b);
	static void Render(int w_sizex, int w_sizey);
	static void SetScale(float scale_x, float scale_y);

private:
	static int GetValueID(const char& value, int* clip_start, int* clip_end);
	static unsigned int LoadTexture(const char* filename);

private:
	static Shader* fontProgram;
	static Shader* switchBackProgram;
	static unsigned int VBO;
	static unsigned int fontTexture;
	static float scale[2];
	static std::vector<fontData>data;
};

