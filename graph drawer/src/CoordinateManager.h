#pragma once

class CoordinateManager
{
public:
	static double GetCenterX();
	static double GetCenterY();
	static double GetScaleLevel();
	static double ScreenToWorldX(int screenX);
	static double ScreenToWorldY(int screenY);
	static int WorldToScreenX(double worldX);
	static int WorldToScreenY(double worldY);
	static float ScreenNormalizedX(int screenX);
	static float ScreenNormalizedY(int screenY);
	static void Update();
private:
	CoordinateManager();

	static double centerX, centerY;
	static double scaleLevel;
	static double panOldX, panOldY;
};