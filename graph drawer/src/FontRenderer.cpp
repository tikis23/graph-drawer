#include "FontRenderer.h"

#include <GL/glew.h>
#include <windows.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "WindowManager.h"
#include "Settings.h"
void FontRenderer::LoadFont(const char* path)
{
	// generate texture
	GLenum format = GL_RGB;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load texture
	int t_nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &width, &height, &t_nrChannels, STBI_rgb_alpha);
	// if texture loaded
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glBindTexture(GL_TEXTURE_2D, textureID);
	}
	else
	{
		MessageBoxA(0, "failed to load font " + *path, "font error", MB_OK);
	}
	stbi_image_free(data);
}

void FontRenderer::RenderFont()
{
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glColor3f(Settings::fontColor[0], Settings::fontColor[1], Settings::fontColor[2]);
	for (int i = 0; i < glyphs.size(); i++)
	{
		// generate texturePos
		float textureX = 0, textureY = 0;
		for (int j = 0; j < glyphs[i].index; j++)
			textureX += format[j].width;
		textureX = 1.0f / width * textureX;
		float textureWidth = 1.0f / width * format[glyphs[i].index].width;
		float textureHeight = 1.0f / height * format[glyphs[i].index].height;

		// generate screenPos
		float screenPosWidth = 2.0f / WindowManager::GetWindow("main")->GetWidth() * format[glyphs[i].index].width;
		float screenPosHeight = 2.0f / WindowManager::GetWindow("main")->GetHeight() * format[glyphs[i].index].height;

		// draw glyph
		glTexCoord2f(textureX, 0);
		glVertex2f(glyphs[i].posx, glyphs[i].posy);

		glTexCoord2f(textureX + textureWidth, 0);
		glVertex2f(glyphs[i].posx + screenPosWidth, glyphs[i].posy);

		glTexCoord2f(textureX + textureWidth, 1);
		glVertex2f(glyphs[i].posx + screenPosWidth, glyphs[i].posy + screenPosHeight);

		glTexCoord2f(textureX, 1);
		glVertex2f(glyphs[i].posx, glyphs[i].posy + screenPosHeight);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glyphs.clear();
}

void FontRenderer::AddToRenderQueue(std::string glyphString, float posX, float posY)
{
	std::string fontGlyphs = "0123456789.-";
	for (int i = 0; i < glyphString.size(); i++)
	{
		// find glyph index
		unsigned int index = 10;
		for(unsigned int j = 0; j < fontGlyphs.size(); j++)
			if(glyphString[i] == fontGlyphs[j])
			{
				index = j;
				break;
			}
		glyphs.push_back({ index, posX, posY});
		posX += 2.0f / WindowManager::GetWindow("main")->GetWidth() * (format[index].width + 1);
	}
}

unsigned int FontRenderer::textureID;
int FontRenderer::width;
int FontRenderer::height;
std::vector<FontRenderer::glyph> FontRenderer::glyphs;
FontRenderer::glyphFormat FontRenderer::format[] =
{
	{12, 18},
	{6, 18},
	{11, 18},
	{10, 18},
	{11, 18},
	{10, 18},
	{12, 18},
	{10, 18},
	{12, 18},
	{12, 18},
	{3, 18},
	{5, 18}
};