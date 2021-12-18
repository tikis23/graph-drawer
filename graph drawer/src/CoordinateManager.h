#pragma once

class CoordinateManager
{
public:
	static long long int GetCenterX();
	static long long int GetCenterY();
	static long long int GetScaleLevel();
	static long long int ScreenToWorldX(int screenX);
	static long long int ScreenToWorldY(int screenY);
	static int WorldToScreenX(long long int worldX);
	static int WorldToScreenY(long long int worldY);
	static float ScreenNormalizedX(int screenX);
	static float ScreenNormalizedY(int screenY);
	static void Update();
private:
	CoordinateManager();

	static long long int centerX, centerY;
	static long long int scaleLevel;
	static long long int panOldX, panOldY;
};