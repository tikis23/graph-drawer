#pragma once
#include <string>
#include <vector>
class FontRenderer
{
public:
	static void LoadFont(const char* path);
	static void RenderFont();
	static void AddToRenderQueue(std::string glyphString, float posX, float posY);
private:
	FontRenderer();
	struct glyph
	{
		unsigned int index;
		float posx, posy;
	};
	struct glyphFormat
	{
		int width;
		int height;
	};
	static unsigned int textureID;
	static int width, height;
	static std::vector<glyph> glyphs;
	static glyphFormat format[];
};