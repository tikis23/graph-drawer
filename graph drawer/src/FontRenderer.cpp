#include "FontRenderer.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
void FontRenderer::Setup(Shader* switchBackShader)
{
	switchBackProgram = switchBackShader;
	fontProgram = new Shader;
	fontProgram->LoadShaders("Font", "res/shaders/font.vs", "res/shaders/font.fs");
	glGenBuffers(1, &VBO);
	fontTexture = LoadTexture("res/fonts/FontDefault.bmp");
}

void FontRenderer::Exit()
{
	delete fontProgram;
	glDeleteBuffers(1, &VBO);
}

void FontRenderer::Draw(std::string value, int x, int y, int offset, float color_r, float color_g, float color_b)
{
	float current_offset_x = x*scale[0]-x;
	float current_offset_y = y*scale[1]-y;

	for (int i = 0; i < value.size(); i++)
	{
		int clip_start, clip_end;
		int id = GetValueID(value[i], &clip_start, &clip_end);
		fontData temp;
		temp.color[0] = color_r;
		temp.color[1] = color_g;
		temp.color[2] = color_b;

		// BOTTOM_LEFT
		temp.position[0] = x * scale[0] - current_offset_x;
		temp.position[1] = y * scale[1] - current_offset_y;
		temp.position[2] = (float)id / 12 + (float)clip_start / 768;
		temp.position[3] = 0; 
		data.push_back(temp);

		// TOP_LEFT
		temp.position[0] = x * scale[0] - current_offset_x;
		temp.position[1] = (y + 64) * scale[1] - current_offset_y;
		temp.position[2] = (float)id / 12 + (float)clip_start / 768;
		temp.position[3] = 1;

		data.push_back(temp);

		// TOP_RIGHT
		temp.position[0] = (x + 64 - clip_start - clip_end) * scale[0] - current_offset_x;
		temp.position[1] = (y + 64) * scale[1] - current_offset_y;
		temp.position[2] = (float)(id + 1) / 12 - (float)clip_end / 768;
		temp.position[3] = 1;

		data.push_back(temp);

		// TOP_RIGHT
		temp.position[0] = (x + 64 - clip_start - clip_end) * scale[0] - current_offset_x;
		temp.position[1] = (y + 64) * scale[1] - current_offset_y;
		temp.position[2] = (float)(id + 1) / 12 - (float)clip_end / 768;
		temp.position[3] = 1;

		data.push_back(temp);

		// BOTTOM_RIGHT
		temp.position[0] = (x + 64 - clip_start - clip_end) * scale[0] - current_offset_x;
		temp.position[1] = y * scale[1] - current_offset_y;
		temp.position[2] = (float)(id + 1) / 12 - (float)clip_end / 768;
		temp.position[3] = 0;

		data.push_back(temp);

		// BOTTOM_LEFT
		temp.position[0] = (float)x * scale[0] - current_offset_x;
		temp.position[1] = (float)y * scale[1] - current_offset_y;
		temp.position[2] = (float)id / 12 + (float)clip_start / 768;
		temp.position[3] = 0;

		data.push_back(temp);
		x += (64 - clip_start - clip_end + offset);

	}
}

void FontRenderer::Render(int w_sizex, int w_sizey)
{
	if (data.size() > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(fontData), &data[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(fontData), (void*)offsetof(fontData, position));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(fontData), (void*)offsetof(fontData, color));
		glEnableVertexAttribArray(1);

		fontProgram->Use();
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, fontTexture);
		fontProgram->setUniform1i("fontTexture", 0);
		fontProgram->setUniform2f("u_windowSize", w_sizex, w_sizey);
		glDrawArrays(GL_TRIANGLES, 0, data.size());
		switchBackProgram->Use();
		data.clear();
	}
}

void FontRenderer::SetScale(float scale_x, float scale_y)
{
	scale[0] = scale_x;
	scale[1] = scale_y;
}

int FontRenderer::GetValueID(const char& value, int* clip_start, int* clip_end)
{
	if (value == '0')
	{
		*clip_start = 5;
		*clip_end = 7;
		return 0;
	}
	if (value == '1')
	{
		*clip_start = 12;
		*clip_end = 25;
		return 1;
	}
	if (value == '2')
	{
		*clip_start = 7;
		*clip_end = 8;
		return 2;
	}
	if (value == '3')
	{
		*clip_start = 7;
		*clip_end = 10;
		return 3;
	}
	if (value == '4')
	{
		*clip_start = 1;
		*clip_end = 4;
		return 4;
	}
	if (value == '5')
	{
		*clip_start = 10;
		*clip_end = 10;
		return 5;
	}
	if (value == '6')
	{
		*clip_start = 6;
		*clip_end = 7;
		return 6;
	}
	if (value == '7')
	{
		*clip_start = 6;
		*clip_end = 8;
		return 7;
	}
	if (value == '8')
	{
		*clip_start = 5;
		*clip_end = 7;
		return 8;
	}
	if (value == '9')
	{
		*clip_start = 5;
		*clip_end = 8;
		return 9;
	}
	if (value == '.')
	{
		*clip_start = 6;
		*clip_end = 46;
		return 10;
	}
	if (value == '-')
	{
		*clip_start = 3;
		*clip_end = 35;
		return 11;
	}

	return 0;
}

unsigned int FontRenderer::LoadTexture(const char* filename)
{
	GLenum format = GL_RGB;
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load texture
	int t_width, t_height, t_nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filename, &t_width, &t_height, &t_nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, format, t_width, t_height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "failed to load texture " << filename << std::endl;
	}
	stbi_image_free(data);
	return texture;
}

Shader* FontRenderer::fontProgram;
Shader* FontRenderer::switchBackProgram;
unsigned int FontRenderer::VBO;
unsigned int FontRenderer::fontTexture;
float FontRenderer::scale[2];
std::vector<fontData> FontRenderer::data;