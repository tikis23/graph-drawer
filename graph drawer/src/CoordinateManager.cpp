#include "CoordinateManager.h"

#include "Input.h"
#include "WindowManager.h"
#include "Settings.h"

long long int CoordinateManager::GetCenterX()
{
    return centerX;
}

long long int CoordinateManager::GetCenterY()
{
    return centerY;
}

long long int CoordinateManager::GetScaleLevel()
{
    return scaleLevel;
}

long long int CoordinateManager::ScreenToWorldX(int screenX)
{
    return (centerX + (-scaleLevel + ((scaleLevel + scaleLevel) / (WindowManager::GetWindow("main")->GetWidth())) * (screenX))) / Settings::globalScale;
}

long long int CoordinateManager::ScreenToWorldY(int screenY)
{
    return (centerY + (-scaleLevel + ((scaleLevel + scaleLevel) / (WindowManager::GetWindow("main")->GetHeight())) * (screenY))) / Settings::globalScale;
}

int CoordinateManager::WorldToScreenX(long long int worldX)
{
    return (WindowManager::GetWindow("main")->GetWidth() / ((centerX + scaleLevel) - (centerX - scaleLevel))) * (Settings::globalScale * worldX - (centerX - scaleLevel));
}

int CoordinateManager::WorldToScreenY(long long int worldY)
{
    return (WindowManager::GetWindow("main")->GetHeight() / ((centerY + scaleLevel) - (centerY - scaleLevel))) * (Settings::globalScale * worldY - (centerY - scaleLevel));
}

float CoordinateManager::ScreenNormalizedX(int screenX)
{
    return 2.0f * (screenX + 0.5f) / WindowManager::GetWindow("main")->GetWidth() - 1.0f;
}
float CoordinateManager::ScreenNormalizedY(int screenY)
{
    return 2.0f * (screenY + 0.5f) / WindowManager::GetWindow("main")->GetHeight() - 1.0f;
}

void CoordinateManager::Update()
{
    GLFWwindow* window = WindowManager::GetWindow("main")->GetHandle();

    // panning
    int tempX, tempY;
    Input::GetMousePosition(window, tempX, tempY);
    long long panX = ScreenToWorldX(tempX);
    long long panY = ScreenToWorldY(tempY);
    if (Input::GetMouseButton(window, Input::Mouse::LEFT, Input::Action::HOLD))
    {
        long long int panDiffX = panX - panOldX;
        long long int panDiffY = panY - panOldY;
        centerX -= panDiffX * Settings::globalScale;
        centerY -= panDiffY * Settings::globalScale;
    }
    panOldX = panX;
    panOldY = panY;

    // zooming
    if (Input::GetScrollOffsetY(window) != 0)
    {
        // check for offset
        Input::GetMousePosition(window, tempX, tempY);
        long long int oldMouseWorldX = ScreenToWorldX(tempX);
        long long int oldMouseWorldY = ScreenToWorldY(tempY);

        // scale
        scaleLevel -= (double)Input::GetScrollOffsetY(window) * scaleLevel * 0.01;
        if (scaleLevel < 10)
            scaleLevel = 10;

        // remove offset
        Input::GetMousePosition(window, tempX, tempY);
        long long int mouseWorldX = ScreenToWorldX(tempX);
        long long int mouseWorldY = ScreenToWorldY(tempY);
        centerX += oldMouseWorldX - mouseWorldX;
        centerY += oldMouseWorldY - mouseWorldY;
    }
}

long long int CoordinateManager::centerX = -400;
long long int CoordinateManager::centerY = 0;
long long int CoordinateManager::scaleLevel = 10000;
long long int CoordinateManager::panOldX;
long long int CoordinateManager::panOldY;