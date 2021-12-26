#pragma once
#include <string>
#include <vector>
#include <unordered_map>
class FontRenderer
{
public:
	static void LoadFont(const char* fontPath);
	static void RenderFont();
	static void AddToRenderQueue(std::string glyphString, int posx, int posy);
private:
	FontRenderer();
	static bool LoadFontData(const char* pathFontData);
	// format structs
	struct f_info
	{
		std::string face;
		int size;
		bool bold;
		bool italic;
		std::string charset;
		bool unicode;
		int stretchH;
		bool smooth;
		int aa;
		int padding[4];
		int spacing[2];
		int outline;
	};
	struct f_common
	{
		int lineHeight;
		int base;
		int scaleW;
		int scaleH;
		int pages;
		bool packed;
		int alphaChnl;
		int redChnl;
		int greenChnl;
		int blueChnl;
	};
	struct f_page
	{
		int id;
		std::string file;
	};
	struct f_char
	{
		char id;
		int x;
		int y;
		int width;
		int height;
		int xOffset;
		int yOffset;
		int xAdvance;
		int page;
		int chnl;
	};
	struct f_kerning
	{
		char first;
		char second;
		bool operator==(const f_kerning& other) const
		{
			return (first == other.first && second == other.second);
		}
	};

	// rendering structs
	struct r_glyph
	{
		float posX0, posY0;
		float posX1, posY1;
		float texX0, texY0;
		float texX1, texY1;
	};
	struct r_line
	{
		std::vector<r_glyph> glyphs;
	};

	// hasher
	struct KeyHasher
	{
		std::size_t operator()(const f_kerning& k) const
		{
			using std::size_t;
			using std::hash;
			using std::string;

			return ((hash<char>()(k.first) ^ (hash<char>()(k.second) << 1)) >> 1);
		}
	};
private:
	static unsigned int textureID;
	static f_info _info;
	static f_common _common;
	static f_page _page;
	static std::unordered_map<char, f_char> _char;
	static std::unordered_map<f_kerning, int, KeyHasher> _kerning;

	static std::vector<r_line> renderQueue;
};