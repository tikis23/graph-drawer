#pragma once

class Settings
{
public:
	static float globalScale;
	static float backgroundColor[3];
	static float gridColor[3];
	static float fontColor[3];
	static bool drawGrid;
	static float panSpeed;
	static float zoomSpeed;
private:
	Settings();
};