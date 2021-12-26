#include "FontRenderer.h"

#include <GL/glew.h>
#include <windows.h>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "WindowManager.h"
#include "Settings.h"
#include "CoordinateManager.h"

void FontRenderer::LoadFont(const char* fontPath)
{
	if (!LoadFontData(fontPath))
		return;

	// generate texture
	GLenum format = GL_RGBA;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load texture
	std::string imagePath = std::string(fontPath).substr(0, std::string(fontPath).find_last_of('/') + 1) + _page.file;
	int w, h, c;
	unsigned char* data = stbi_load(imagePath.c_str(), &w, &h, &c, STBI_rgb_alpha);
	// if texture loaded
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, format, _common.scaleW, _common.scaleH, 0, format, GL_UNSIGNED_BYTE, data);
		glBindTexture(GL_TEXTURE_2D, textureID);
	}
	else
	{
		MessageBoxA(0, ("failed to load font " + imagePath).c_str(), "font error", MB_OK);
	}
	stbi_image_free(data);
}

void FontRenderer::RenderFont()
{
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glColor3f(Settings::fontColor[0], Settings::fontColor[1], Settings::fontColor[2]);
	for (int i = 0; i < renderQueue.size(); i++)
	{
		for (int j = 0; j < renderQueue[i].glyphs.size(); j++)
		{
			glTexCoord2f(renderQueue[i].glyphs[j].texX0, renderQueue[i].glyphs[j].texY0);
			glVertex2f(renderQueue[i].glyphs[j].posX0, renderQueue[i].glyphs[j].posY0);
			glTexCoord2f(renderQueue[i].glyphs[j].texX0, renderQueue[i].glyphs[j].texY1);
			glVertex2f(renderQueue[i].glyphs[j].posX0, renderQueue[i].glyphs[j].posY1);
			glTexCoord2f(renderQueue[i].glyphs[j].texX1, renderQueue[i].glyphs[j].texY1);
			glVertex2f(renderQueue[i].glyphs[j].posX1, renderQueue[i].glyphs[j].posY1);
			glTexCoord2f(renderQueue[i].glyphs[j].texX1, renderQueue[i].glyphs[j].texY0);
			glVertex2f(renderQueue[i].glyphs[j].posX1, renderQueue[i].glyphs[j].posY0);
		}
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	renderQueue.clear();
}

void FontRenderer::AddToRenderQueue(std::string glyphString, int posx, int posy)
{
	posy -= 2;
	renderQueue.push_back({});
	for (int i = 0; i < glyphString.size(); i++)
	{
		r_glyph glyph;
		// get current char
		f_char* currentChar = &_char.at(glyphString[i]);
		if (currentChar == nullptr)
			continue;

		// kerning
		if (i - 1 >= 0)
		{
			auto kerning = _kerning.find({ glyphString[i - 1], glyphString[i] });
			if (kerning != _kerning.end())
				posx += kerning->second;
		}

		// position
		glyph.posX0 = CoordinateManager::ScreenNormalizedX(posx);
		glyph.posY0 = CoordinateManager::ScreenNormalizedX(posy);
		glyph.posX1 = CoordinateManager::ScreenNormalizedX(posx + currentChar->width);
		glyph.posY1 = CoordinateManager::ScreenNormalizedX(posy + currentChar->height);

		// texture
		glyph.texX0 = (float)currentChar->x / _common.scaleW;
		glyph.texY0 = (float)(currentChar->y + currentChar->height) / _common.scaleH;
		glyph.texX1 = (float)(currentChar->x + currentChar->width) / _common.scaleW;
		glyph.texY1 = (float)currentChar->y / _common.scaleH;

		// advance
		posx += currentChar->xAdvance;

		renderQueue.back().glyphs.push_back(glyph);
	}
}

bool FontRenderer::LoadFontData(const char* pathFontData)
{
	std::ifstream file;
	file.open(pathFontData, std::fstream::in);
	if (!file.is_open())
		return false;
	std::string temp;
	///////////////////////////////////////////////////////////////////// load info
	file >> temp;
	// face
	file >> temp;
	_info.face = temp.substr(temp.find_first_of('"') + 1, temp.find_last_of('"') - temp.find_first_of('"') - 1);
	if(std::count(temp.begin(), temp.end(), '"') != 2)
	{
		temp = "";
		while (std::count(temp.begin(), temp.end(), '"') != 1)
		{
			file >> temp;
			_info.face += " " + temp.substr(0, temp.find_last_of('"') - 1);
		}
	}
	// size
	file >> temp;
	_info.size = std::stoi(temp.substr(temp.find_first_of('=') + 1));
	// bold
	file >> temp;
	_info.bold = std::stoi(temp.substr(temp.find_first_of('=') + 1));
	// italic
	file >> temp;
	_info.italic = std::stoi(temp.substr(temp.find_first_of('=') + 1));
	// charset
	file >> temp;
	_info.charset = temp.substr(temp.find_first_of('"') + 1, temp.find_last_of('"') - temp.find_first_of('"') - 1);
	if (std::count(temp.begin(), temp.end(), '"') != 2)
	{
		temp = "";
		while (std::count(temp.begin(), temp.end(), '"') != 1)
		{
			file >> temp;
			_info.charset += " " + temp.substr(0, temp.find_last_of('"') - 1);
		}
	}
	// unicode
	file >> temp;
	_info.unicode = std::stoi(temp.substr(temp.find_first_of('=') + 1));
	// stretchH
	file >> temp;
	_info.stretchH = std::stoi(temp.substr(temp.find_first_of('=') + 1));
	// smooth
	file >> temp;
	_info.smooth = std::stoi(temp.substr(temp.find_first_of('=') + 1));
	// aa
	file >> temp;
	_info.aa = std::stoi(temp.substr(temp.find_first_of('=') + 1));
	// padding
	file >> temp;
	temp = temp.substr(temp.find_first_of('=') + 1);
	for (int i = 0; i < 4; i++)
	{
		_info.padding[i] = std::stoi(temp);
		temp = temp.substr(temp.find_first_of(',') + 1);
	}
	// spacing
	file >> temp;
	temp = temp.substr(temp.find_first_of('=') + 1);
	_info.spacing[0] = std::stoi(temp);
	temp = temp.substr(temp.find_first_of(',') + 1);
	_info.spacing[1] = std::stoi(temp);
	// outline
	_info.outline = std::stoi(temp.substr(temp.find_first_of('=') + 1));
	file >> temp;

	///////////////////////////////////////////////////////////////////// load common
	file >> temp;
	// lineHeight
	file >> temp;
	_common.lineHeight = std::stoi(temp.substr(temp.find_first_of('=') + 1));
	// base
	file >> temp;
	_common.base = std::stoi(temp.substr(temp.find_first_of('=') + 1));
	// scaleW
	file >> temp;
	_common.scaleW = std::stoi(temp.substr(temp.find_first_of('=') + 1));
	// scaleH
	file >> temp;
	_common.scaleH = std::stoi(temp.substr(temp.find_first_of('=') + 1));
	// pages
	file >> temp;
	_common.pages = std::stoi(temp.substr(temp.find_first_of('=') + 1));
	// packed
	file >> temp;
	_common.packed = std::stoi(temp.substr(temp.find_first_of('=') + 1));
	// alphaChnl
	file >> temp;
	_common.alphaChnl = std::stoi(temp.substr(temp.find_first_of('=') + 1));
	// redChnl
	file >> temp;
	_common.redChnl = std::stoi(temp.substr(temp.find_first_of('=') + 1));
	// greenChnl
	file >> temp;
	_common.greenChnl = std::stoi(temp.substr(temp.find_first_of('=') + 1));
	// blueChnl
	file >> temp;
	_common.blueChnl = std::stoi(temp.substr(temp.find_first_of('=') + 1));

	///////////////////////////////////////////////////////////////////// load page
	file >> temp;
	// id
	file >> temp;
	_page.id = std::stoi(temp.substr(temp.find_first_of('=') + 1));
	// file
	file >> temp;
	_page.file = temp.substr(temp.find_first_of('"') + 1, temp.find_last_of('"') - temp.find_first_of('"') - 1);

	///////////////////////////////////////////////////////////////////// load char
	file >> temp;
	// count
	file >> temp;
	int count = std::stoi(temp.substr(temp.find_first_of('=') + 1));
	_char.clear();
	for (int i = 0; i < count; i++)
	{
		file >> temp;
		// id
		file >> temp;
		int id = std::stoi(temp.substr(temp.find_first_of('=') + 1));
		_char[id].id = id;
		// x
		file >> temp;
		_char[id].x = std::stoi(temp.substr(temp.find_first_of('=') + 1));
		// y
		file >> temp;
		_char[id].y = std::stoi(temp.substr(temp.find_first_of('=') + 1));
		// width
		file >> temp;
		_char[id].width = std::stoi(temp.substr(temp.find_first_of('=') + 1));
		// height
		file >> temp;
		_char[id].height = std::stoi(temp.substr(temp.find_first_of('=') + 1));
		// xOffset
		file >> temp;
		_char[id].xOffset = std::stoi(temp.substr(temp.find_first_of('=') + 1));
		// yOffset
		file >> temp;
		_char[id].yOffset = std::stoi(temp.substr(temp.find_first_of('=') + 1));
		// xAdvance
		file >> temp;
		_char[id].xAdvance = std::stoi(temp.substr(temp.find_first_of('=') + 1));
		// page
		file >> temp;
		_char[id].page = std::stoi(temp.substr(temp.find_first_of('=') + 1));
		// chnl
		file >> temp;
		_char[id].chnl = std::stoi(temp.substr(temp.find_first_of('=') + 1));
	}

	///////////////////////////////////////////////////////////////////// load kerning
	file >> temp;
	// count
	file >> temp;
	count = std::stoi(temp.substr(temp.find_first_of('=') + 1));
	_kerning.clear();
	for (int i = 0; i < count; i++)
	{
		file >> temp;
		f_kerning kerning;
		// first
		file >> temp;
		kerning.first = std::stoi(temp.substr(temp.find_first_of('=') + 1));
		// second
		file >> temp;
		kerning.second = std::stoi(temp.substr(temp.find_first_of('=') + 1));
		// amount
		file >> temp;
		_kerning[kerning] = std::stoi(temp.substr(temp.find_first_of('=') + 1));
	}
	file.close();
	return true;
}

unsigned int FontRenderer::textureID;
FontRenderer::f_info FontRenderer::_info;
FontRenderer::f_common FontRenderer::_common;
FontRenderer::f_page FontRenderer::_page;
std::unordered_map<char, FontRenderer::f_char> FontRenderer::_char;
std::unordered_map<FontRenderer::f_kerning, int, FontRenderer::KeyHasher> FontRenderer::_kerning;
std::vector<FontRenderer::r_line> FontRenderer::renderQueue;
